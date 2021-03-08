//
//  EdlundMaze.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/3/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "EdlundMazeWorld.hpp"

void EdlundMazeWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    //showMaze();
    //exit(0);
    if(analyze){
        for(auto organism:population->population){
            evaluateSolo(organism,analyze,visualize);
        }
    } else {
        for(int reps=0;reps<4;reps++){
            makeNewMaze();
            for(auto organism:population->population){
                evaluateSolo(organism,analyze,visualize);
            }
        }
    }
}

void EdlundMazeWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    int x=0;
    int y=7;
    organism->brain->resetBrain();
    maze->resetRecordingToFirstTick();
    for(int t=0;t<1024;t++){
        organism->brain->setInput(0, maze->get(x&255,y-1));
        organism->brain->setInput(1, maze->get((x+1)&255,y-1)&1);
        organism->brain->setInput(2, maze->get((x+1)&255,y)&1);
        organism->brain->setInput(3, maze->get((x+1)&255,y+1)&1);
        organism->brain->setInput(4, maze->get(x&255,y+1));
        organism->brain->setInput(5, maze->get(x&255,y)>>1);
        organism->brain->update();
        int action=bit(organism->brain->getOutput(0))+(bit(organism->brain->getOutput(1))<<1);
        //action=Random::getInt(0,3);
        maze->set(x&255,y,maze->get(x&255,y)&3);
        switch(action){
            case 0:
                break;
            case 1:
                if((y>1)&&((maze->get(x&255,y-1)&1)==0))
                    y--;
                break;
            case 2:
                if((y<14)&&((maze->get(x&255,y+1)&1)==0))
                    y++;
                break;
            case 3:
                if((maze->get((x+1)&255,y)&1)==0)
                    x++;
                break;
        }
        maze->set(x&255,y,maze->get(x&255,y)+4);
        maze->tick();
    }
    /*
    if(x>200){
        maze->save("test.csv");
        exit(0);
    }
    */
    organism->dataCollector->addValue("score",(double)x);
}

void EdlundMazeWorld::makeNewMaze(){
    doorPlacement.clear();
    for(int i=0;i<128;i++){
        doorPlacement.push_back(Random::getInt(1, 14));
    }
    maze=std::make_shared<Area>(256,16);
    for(int i=0;i<256;i++){
        for(int j=0;j<16;j++){
            if((j==0)||(j==15)||((i&1)==1))
                maze->set(i,j,1);
        }
        if(((i&1)==1)){
            if(doorPlacement[((i+2)>>1)%128]>doorPlacement[i>>1])
                maze->set(i,doorPlacement[i>>1],2);
            else
                maze->set(i,doorPlacement[i>>1],0);
        }
    }
    
}

void EdlundMazeWorld::showMaze(){
    for(int j=0;j<16;j++){
        for(int i=0;i<60;i++){
            std::cout<<maze->get(i,j);
        }
        std::cout<<std::endl;
    }
}
