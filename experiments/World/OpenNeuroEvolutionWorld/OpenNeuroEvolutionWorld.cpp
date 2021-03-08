//
//  OpenNeuroEvolutionWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "OpenNeuroEvolutionWorld.hpp"

#define _empty 0
#define _agent 1
#define _food 2
#define _rock 3

double triangleSin(double x,int resolution){
    double Y=0.0;
    for(int i=0;i<resolution;i++){
        double n=(2*i)+1;
        Y+=pow(-1,(double)i)*pow(n,-2.0)*sin(n*x);
    }
    return (0.25*PI)*Y;
}

OpenNeuroEvolutionWorld::OpenNeuroEvolutionWorld(){
    width=512;
    height=512;

    taskSwitchingCostParams.resize(4);
    for(int i=0;i<4;i++){
        taskSwitchingCostParams[i].resize(4);
        for(int j=0;j<4;j++)
            taskSwitchingCostParams[i][j]=std::pair<double,double>(Random::getFloat(0.0, 1.0),Random::getFloat(0.0, 1.0));
    }
    std::string S="score,hits,bites";
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++){
            S+=","+std::to_string(i)+"_"+std::to_string(j);
            S+=",""reward_"+std::to_string(i)+"_"+std::to_string(j);
        }
    Parameters::setParameter("LODManager-parametersToSave",S);
}

void OpenNeuroEvolutionWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    int xm[4]={0,1,0,-1};
    int ym[4]={-1,-0,1,0};
    std::vector<std::shared_ptr<Agent>> agents;
    for(int i=0;i<population->population.size();i++){
        std::shared_ptr<Agent> A=std::make_shared<Agent>(Random::getInt(2,width-3),Random::getInt(2,height-3),Random::getInt(0,3),
                                                         population->population[i]);
        agents.push_back(A);
    }
    ResetWorld();
    updateRewardMatrix((double)Utilities::currentGeneration*Parameters::getDouble(std::string("OpenNeuroEvolutionWorld-speedScaling"), 0.001));
    for(int t=0;t<1000;t++){
        area->tick();
        for(auto who:agents){
            area->set(who->xPos,who->yPos,_empty);
            whoArea[who->xPos][who->yPos]=nullptr;
            int front=area->get(who->xPos+xm[who->dir],who->yPos+ym[who->dir]);
            for(int i=0;i<4;i++)
                who->organism->brain->setInput(i, (front>>i)&1);
            who->organism->brain->update();
            int action=0;//Random::getInt(0,15);
            for(int i=0;i<4;i++)
                action=(action<<1)+bit(who->organism->brain->getOutput(i));
            switch(action&3){
                case 0: break;
                case 1: who->dir=(who->dir+1)&3; break;
                case 2: who->dir=(who->dir-1)&3; break;
                case 3:{
                    switch(front&3){
                        case _food:{
                            int targetFood=(front>>2)&3;
                            if(who->lastFoodEaten==-1)
                                who->lastFoodEaten=targetFood;
                            who->switches[who->lastFoodEaten][targetFood]++;
                            who->collected+=taskSwitchingCost[who->lastFoodEaten][targetFood];
                            if(targetFood!=who->lastFoodEaten)
                                who->lastFoodEaten=targetFood;
                        }
                        case _empty:
                            who->xPos+=xm[who->dir];
                            who->yPos+=ym[who->dir];
                            break;
                        case _agent:
                        {
                            std::shared_ptr<Agent> opponent=whoArea[who->xPos+xm[who->dir]][who->yPos+ym[who->dir]];
                            if(opponent->collected>0.1){
                                who->collected+=0.1;
                                who->bites++;
                                opponent->collected-=0.1;
                                opponent->hits++;
                            }
                        }
                            break;
                    }
                }
                    break;
            }
            area->set(who->xPos,who->yPos,_agent+(action&12));
            whoArea[who->xPos][who->yPos]=who;
        }
    }
    for(auto who:agents){
        if(who->collected<0.0)
            who->collected=0.0;
        who->organism->dataCollector->setValue("score",who->collected);
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++){
                who->organism->dataCollector->addValue(std::to_string(i)+"_"+std::to_string(j), who->switches[i][j]);
                who->organism->dataCollector->addValue("reward_"+std::to_string(i)+"_"+std::to_string(j), taskSwitchingCost[i][j]);
            }
        who->organism->dataCollector->setValue("bites",(int)who->bites);
        who->organism->dataCollector->setValue("hits",(int)who->hits);
    }

}

int OpenNeuroEvolutionWorld::nrOfSensors(){
    return 4;
}

int OpenNeuroEvolutionWorld::nrOfMotors(){
    return 4;
}

void OpenNeuroEvolutionWorld::ResetWorld(){
    area=std::make_shared<Area>(width,height);
    whoArea.resize(width);
    for(int x=0;x<width;x++){
        whoArea[x].resize(height);
        for(int y=0;y<height;y++){
            area->set(x,y,_empty);
            if((x==0)||(y==0)||(x==(width-1))||(y==(height-1)))
                area->set(x,y,_rock);
        }
    }
    for(int i=0;i<4;i++)
        for(int j=0;j<10000;j++){
            area->set(Random::getInt(1,width-2),Random::getInt(1,height-2),_food+(i<<2));
        }
    for(int j=0;j<10000;j++){
        area->set(Random::getInt(1,width-2),Random::getInt(1,height-2),_rock);
    }
}

void OpenNeuroEvolutionWorld::updateRewardMatrix(double t){
    taskSwitchingCost.resize(4);
    for(int i=0;i<4;i++){
        taskSwitchingCost[i].resize(4);
        for(int j=0;j<4;j++){
            taskSwitchingCost[i][j]=triangleSin((t*(taskSwitchingCostParams[i][j].second+0.5))+(taskSwitchingCostParams[i][j].first*PI*2.0),10);
        }
    }
}

/*********   AGENT *********/
Agent::Agent(int startX,int startY,int startDir,std::shared_ptr<Organism> who){
    xPos=startX;
    yPos=startY;
    dir=startDir;
    organism=who;
    hits=0;
    bites=0;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            switches[i][j]=0;
    collected=0.0;
    lastFoodEaten=-1;
}

Agent::~Agent(){
}
