//
//  ArrowMazeWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/1/20.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "ArrowMazeWorld.hpp"

#define _wall 4
#define _agent 1
#define _empty 0

ArrowMazeWorld::ArrowMazeWorld(){
    std::string S=std::string("score");
    for(int m=0;m<24;m++)
        S+=std::string(",M_")+std::to_string(m);
    Parameters::setParameter("LODManager-parametersToSave", S);
}

void ArrowMazeWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    do{
        area=std::make_shared<Area>(48,48,_empty);
        for(int i=0;i<48;i++){
            area->set(i,0,_wall);
            area->set(i,47,_wall);
            area->set(0,i,_wall);
            area->set(47,i,_wall);
        }
        for(int i=0;i<48*48*Parameters::getDouble("ArrowMazeWorld-wallDensity", 0.1);i++)
            area->set(Random::getInt(1, 46),Random::getInt(1, 46),_wall);
        targetX=Random::getInt(16, 32);
        targetY=Random::getInt(16, 32);
        area->set(targetX,targetY,_empty);
        //area->show();
        dist=area->computeDijkstraPath(targetX,targetY,0);
        startLocations.clear();
        for(int i=1;i<47;i++)
            for(int j=1;j<47;j++)
                if(dist[i][j]==28)
                    startLocations.push_back(std::pair<int,int>(i,j));
    }while(startLocations.size()<24);
    for(int i=1;i<47;i++)
        for(int j=1;j<47;j++)
            if(area->get(i,j)==_empty){
                int nd=0;
                int sd=1000;
                for(int k=0;k<4;k++){
                    int ld=dist[i+area->xm[k]][j+area->ym[k]];
                    if(ld!=-1)
                        if(ld<sd){
                            sd=ld;
                            nd=k;
                        }
                }
                area->set(i,j,nd);
            }
    
    area->setAsStartConfig();
    for(auto organism:population->population)
        organism->dataCollector->setValue("score",1.0);
    for(auto organism:population->population){
        for(int m=0;m<24;m++)
            evaluateSolo(organism,m,analyze,visualize);
    }
    area->save("areaRecord.csv");
}

void ArrowMazeWorld::evaluateSolo(std::shared_ptr<Organism> organism,int mappingID,bool analyze,bool visualize){
    auto brain=organism->brain;
    brain->resetBrain();
    area->resetToStartConfig();
    int xPos,yPos,dir=Random::getInt(0,3);
    int sl=Random::getInt(0, (int)startLocations.size()-1);
    double score=0.0;
    int targetReached=0;
    xPos=startLocations[sl].first;
    yPos=startLocations[sl].second;
    area->set(xPos,yPos,area->get(xPos,yPos)+100);
    for(int t=0;t<256;t++){
        int under=(area->get(xPos, yPos)-dir)&3;
        //brain->setInput(0, under&1);
        //brain->setInput(1, (under>>1)&1);
        brain->setInput(under,1.0);
        brain->update();
        int action=bit(brain->getOutput(0))+(bit(brain->getOutput(1))<<1);
        action=mappings[mappingID][action];
        area->set(xPos,yPos,area->get(xPos,yPos)-100);
        switch(action){
            case 0:
                break;
            case 1:
                dir=(dir+1)&3;
                break;
            case 2:
                dir=(dir-1)&3;
                break;
            case 3:
                if(dist[xPos+area->xm[dir]][yPos+area->ym[dir]]!=-1){
                    xPos+=area->xm[dir];
                    yPos+=area->ym[dir];
                }
        }
        
        score+=1.0/(double)(dist[xPos][yPos]+1);
        if((xPos==targetX)&&(yPos==targetY)){
            sl=Random::getInt(0, (int)startLocations.size()-1);
            xPos=startLocations[sl].first;
            yPos=startLocations[sl].second;
            dir=Random::getInt(0,3);
            score+=200.0;
            targetReached++;
        }
        area->set(xPos,yPos,area->get(xPos,yPos)+100);
        area->tick();
    }
    organism->dataCollector->multValue("score",score/10.0);
    organism->dataCollector->addValue(std::string("M_"+std::to_string(mappingID)),targetReached);
}

int ArrowMazeWorld::nrOfSensors(){
    return 4;
}

int ArrowMazeWorld::nrOfMotors(){
    return 2;
}
