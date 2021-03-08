//
//  TheoryOfMindWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "TheoryOfMindWorld.hpp"

#define _empty 0
#define _wall 1
#define _lever 2
#define _agent 4
#define _red 8
#define _blue 16
#define _door 32

TheoryOfMindWorld::TheoryOfMindWorld(){
    area=std::make_shared<Area>(13,13,_empty);
    for(int i=0;i<13;i++)
        for(int j=0;j<13;j++)
            area->set(i,j,_wall);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            area->set(5+i,j+1,_empty);
            area->set(5+i,j+5,_empty);
            area->set(5+i,j+9,_empty);
            area->set(1+i,j+5,_empty);
            area->set(9+i,j+5,_empty);
        }
    }
    area->set(4,6,_door);
    area->set(8,6,_door);
    area->set(6,4,_door);
    area->set(6,8,_door);
    area->set(1,5,_lever);
    area->set(1,7,_lever);
    area->set(11,5,_lever);
    area->set(11,7,_lever);
    area->setAsStartConfig();
    //Parameters::setParameter("LODManager-parametersToSave", std::string("score,W_0,W_1,W_2,W_3,W_4,W_5,W_6,W_7,W_8"));
    targetWorlds=std::vector<int>({0,1,2,3,4,5,6,7,8});
    std::string S=std::string("score");
    for(auto t:targetWorlds)
        S+=std::string(",W_")+std::to_string(t);
    Parameters::setParameter("LODManager-parametersToSave", S);
}

void TheoryOfMindWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    if(analyze){
        analyzeFile.open(Parameters::getString("TheoryOfMind-analyzeFileName","analyze.csv"));
        shortAnalyzeFile.open(std::string("SHORT_")+Parameters::getString("TheoryOfMind-analyzeFileName","analyze.csv"));
        analyzeFile<<"N,W0,W1,S0_0,S0_1,S0_2,S0_3,S0_4,S0_5,S1_0,S1_1,S1_2,S1_3,S1_4,S1_5,H0_0,H0_1,H0_2,H0_3,H0_4,H0_5,H0_6,H0_7,H0_8,H0_9,H1_0,H1_1,H1_2,H1_3,H1_4,H1_5,H1_6,H1_7,H1_8,H1_9,A0,A1,W,S0,S1,H0,H1"<<std::endl;
        shortAnalyzeFile<<"N,W,S0,S1,H0,H1"<<std::endl;
    }
    for(auto org:population->population){
        area->resetToStartConfig(true);
        double score=0.0;
        for(auto typeOfWorld:targetWorlds){
            double local=0.0;
            for(int startCondition=0;startCondition<4;startCondition++){
                double s=evaluateSoloWorld(org, startCondition,typeOfWorld, analyze, visualize);
                score+=s;
                local+=s;
            }
            org->dataCollector->setValue("W_"+std::to_string(typeOfWorld),local);
        }
        //area->save("testArea.csv");
        org->dataCollector->setValue("score",score);//,pow(1.1,score));
        //exit(0);
    }
    if(analyze)
        analyzeFile.close();
}
double TheoryOfMindWorld::evaluateSoloWorld(std::shared_ptr<Organism> organism,int startCondition,int typeOfWorld, bool analyze,bool visualize){
    std::vector<std::vector<int>> recorder;
    area->resetToStartConfig(false);
    switch(startCondition){
        case 0:
            area->set(1,5,_lever+_red);
            area->set(1,7,_lever+_blue);
            area->set(11,5,_lever+_red);
            area->set(11,7,_lever+_blue);
            break;
        case 1:
            area->set(1,5,_lever+_red);
            area->set(1,7,_lever+_blue);
            area->set(11,5,_lever+_blue);
            area->set(11,7,_lever+_red);
            break;
        case 2:
            area->set(1,5,_lever+_blue);
            area->set(1,7,_lever+_red);
            area->set(11,5,_lever+_red);
            area->set(11,7,_lever+_blue);
            break;
        case 3:
            area->set(1,5,_lever+_blue);
            area->set(1,7,_lever+_red);
            area->set(11,5,_lever+_blue);
            area->set(11,7,_lever+_red);
            break;
    }
    //totally obvious: 1,3
    auto tA=std::vector<int>({_empty,_empty,_empty, _blue,_blue,_blue,  _red, _red,_red});
    auto tB=std::vector<int>({_empty, _blue,  _red,_empty,_blue, _red,_empty,_blue,_red});
    double payoff[9][9]={
        { 0.0, 0.2, 0.2, 0.2, 0.0, 1.0, 0.2, 1.0, 0.0},// N N configuration of room
        { 0.0, 0.2, 0.0, 0.0, 0.0, 0.5, 0.0, 1.0, 0.0},// N B   1
        { 0.0, 0.0, 0.2, 0.0, 0.0, 1.0, 0.0, 0.5, 0.0},// N R   2
        { 0.0, 0.0, 0.0, 0.2, 0.0, 1.0, 0.0, 0.5, 0.0},// B N   3
        { 0.0, 0.5, 0.0, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0},// B B   4
        { 0.0, 0.0, 0.2, 0.2, 0.0, 1.0, 0.0, 0.5, 0.0},// B R   5
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.2, 1.0, 0.0},// R N   6
        { 0.0, 0.2, 0.0, 0.0, 0.0, 0.5, 0.2, 1.0, 0.0},// R B   7
        { 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.5, 1.0, 0.0} //rr
    };
    int fillA=tA[typeOfWorld];
    int fillB=tB[typeOfWorld];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            area->set(5+i,j+1,fillA);
            area->set(5+i,j+9,fillB);
        }
    }
    int xPos[2]={6,6};
    int yPos[2]={1,11};
    int dir[2]={2,0};
    int comm[2]={0,0};
    bool active[2]={true,true};
    int pressed[2]={0,0};
    std::shared_ptr<Brain> brains[2]={organism->brain,organism->brain->clone()};
    brains[0]->resetBrain();
    brains[1]->resetBrain();
    auto order=std::vector<int>({0,1});
    for(int t=0;t<100;t++){
        std::vector<int> states;
        states.resize(41);
        area->tick();
        order[0]=Random::getInt(0,1);
        order[1]=1-order[0];
        for(auto o:order){
            if(active[o]){
                int target=area->get(xPos[o]+xm[dir[o]],yPos[o]+ym[dir[o]]);
                for(int i=0;i<5;i++){
                    brains[o]->setInput(i, (target>>i)&1);
                    if(analyze){
                        states[2+(o*6)+i]=(target>>1)&1;
                    }
                }
               brains[o]->setInput(5, comm[1-o]);
                if(analyze){
                    states[2+(o*6)+5]=comm[1-o];
                    states[37+o]=(target<<1)+comm[1-o];
                }
               brains[o]->update();
           }
        }
        for(auto o:order)
            if(active[o]){
                if((xPos[0]>4)&&(xPos[0]<8)&&(xPos[1]>4)&&(xPos[1]<8)&&
                   (yPos[0]>4)&&(yPos[0]<8)&&(yPos[1]>4)&&(yPos[1]<8))
                    comm[o]=bit(brains[o]->getOutput(2));
                else
                    comm[0]=0;
                int action=bit(brains[o]->getOutput(0))+(bit(brains[0]->getOutput(1))<<1);
                //action=Random::getInt(0,3);
                //action=3;
                switch(action){
                    case 0:
                        break;
                    case 1:
                        dir[o]=(dir[o]+1)&3;
                        break;
                    case 2:
                        dir[o]=(dir[o]-1)&3;
                        break;
                    case 3:{
                            int target=area->get(xPos[o]+xm[dir[o]],yPos[o]+ym[dir[o]]);
                            int source=area->get(xPos[o],yPos[o])&_door;
                        switch(target&7){
                            case _empty:
                                if(source==_empty)
                                    area->set(xPos[o],yPos[o],area->get(xPos[o],yPos[o])&(_door+_red+_blue));
                                else
                                    area->set(xPos[o],yPos[o],_wall);
                                xPos[o]+=xm[dir[o]];
                                yPos[o]+=ym[dir[o]];
                                area->set(xPos[o],yPos[o],_agent+(area->get(xPos[o],yPos[o])&(_door+_red+_blue)));
                                break;
                            case _lever:{
                                active[o]=false;
                                if((target&_blue)==_blue)
                                    pressed[o]=1;
                                else
                                    pressed[o]=2;
                            }
                                break;
                        }
                        }
                        break;
                }
        }
        if(analyze){
            states[0]=tA[typeOfWorld];
            states[1]=tB[typeOfWorld];
            states[36]=typeOfWorld;
            std::vector<double> H;
            int HS=0;
            for(int b=0;b<2;b++){
                H=brains[b]->getHidden();
                for(int a=0;a<10;a++){
                    states[14+(b*10)+a]=bit(H[a]);
                    HS=(HS<<1)+bit(H[a]);
                }
                states[39+b]=HS;
            }
            states[34]=(int)active[0];
            states[35]=(int)active[1];
            recorder.push_back(states);
            analyzeFile<<organism->ID;
            for(auto s:states)
                analyzeFile<<","<<s;
            analyzeFile<<std::endl;
            shortAnalyzeFile<<organism->ID;
            for(int s=36;s<41;s++)
                shortAnalyzeFile<<","<<states[s];
            shortAnalyzeFile<<std::endl;

        }
    }
    int outcome=(pressed[0]*3)+pressed[1];
    /*
    if(payoff[typeOfWorld][outcome]>0)
        std::cout<<typeOfWorld<<" tA:"<<tA[typeOfWorld]<<" tB:"<<tB[typeOfWorld]<<" "<<pressed[0]<<" "<<pressed[1]<<" "<<outcome<<" "<<payoff[typeOfWorld][outcome]<<std::endl;
     */
    return payoff[typeOfWorld][outcome];
}

