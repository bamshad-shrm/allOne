//
//  TestWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "TestWorld.hpp"


void TestWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    int M=Parameters::getInt(std::string("TestWorld-nrOfMotors"),10);
    double score=0.0;
    organism->brain->update();
    for(int m=0;m<M;m++){
        score+=1.0-abs(organism->brain->getOutput(m)-1.0);
        //std::cout<<organism->brain->getOutput(m)<<std::endl;
    }
    organism->dataCollector->setValue("score",score);
}

int TestWorld::nrOfSensors(){
    return 0;
}

int TestWorld::nrOfMotors(){
    int i=Parameters::getInt(std::string("TestWorld-nrOfMotors"),10);
    return i;
}
