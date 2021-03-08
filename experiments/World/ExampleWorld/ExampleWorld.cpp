//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "ExampleWorld.hpp"

//four tile types defined
#define _empty 0
#define _gras 1
#define _agent 2
#define _wall 3

ExampleWorld::ExampleWorld(){
    //initialize the area container to size 16x16 and contain only grass
    area=std::make_shared<Area>(16,16,_gras);
    //adding all walls
    for(int i=0;i<16;i++){
        area->set(0,i,_wall);
        area->set(15,i,_wall);
        area->set(i,0,_wall);
        area->set(i,15,_wall);
    }
    
    area->setAsStartConfig(); // we mark this state as the start configuration, makes resetting easier later
}

void ExampleWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    // simple loop iterating over all indididuals of a population
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
    }
    //we save the last' organisms behavior.
    area->save("areaRecord.csv");
}


//Key function to implement the evaluation of a single organism
void ExampleWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    double score=0.0;
    auto brain=organism->brain;     //retrieve the brain from the organism
    int xPos,yPos,dir;              // define three variables defining the position and facing direction of the agent
    
    brain->resetBrain();            //the brain is reset at "birth"
    area->resetToStartConfig();
    
    xPos=Random::getInt(1, 14);     //random xPos, lower and upper limit are INCLUSIVE!
    yPos=Random::getInt(1, 14);     //random yPos, lower and upper limit are INCLUSIVE!
    dir=Random::getInt(0,3);        //four direction 0-3 = N,E,S,W

    area->set(xPos,yPos,_agent);    //marking the start location of the agent
    
    for(int t=0;t<200;t++){
        int inFront=area->getFromDirDist(xPos, yPos, dir, 1);   //this gives you the tile in front of the  agent
        brain->setInput(0, inFront&1);          //bit magic to set the inputs
        brain->setInput(1, (inFront>>1)&1);     //bit magic to set the inputs
        //now you need to allow the brain a single update "it is thinking now"
        brain->update();
        //now the actions from the brain are querried and translated into a single "action" number
        int action=(bit(brain->getOutput(0))<<1)+bit(brain->getOutput(1));
        //clear the tile the agent stands on
        area->set(xPos,yPos,_empty);
        //executing the actions
        switch(action){
            case 0:
                //do nothing
                break;
            case 1:
                //turn left
                dir=(dir-1)&3;
                break;
            case 2:
                //turn right
                dir=(dir+1)&3;
                break;
            case 3:
                //move forward
                //that depends on what tile was inFront
                switch(inFront){
                    case _gras:
                        score+=1.0; //gras was eaten
                        // NO break -> to run into the next case which moves the agent
                    case _empty:
                        //move forward
                        xPos+=area->xm[dir];
                        yPos+=area->ym[dir];
                        break;
                    case _agent:
                    case _wall:
                        //agent tried to move into a wall or an agent (which should not exist)
                        //do nothing
                        break;
                }
                break;
        }
        //update the area where we stand
        area->set(xPos,yPos,_agent);
        //add a time tick to the agea, so it knows that time progressed
        area->tick();
    }
    
}

int ExampleWorld::nrOfSensors(){
    return 2;
}

int ExampleWorld::nrOfMotors(){
    return 2;
}
