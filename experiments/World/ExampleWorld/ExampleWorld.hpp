//
//  ExampleWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef ExampleWorld_hpp
#define ExampleWorld_hpp

#include "../World.hpp"
#include "../../Core/Area.hpp"

class ExampleWorld:public World{
    //this is a private variable wrapping a vector<vector<int>>
    //insuch a way that we can log all changes to it
    //this simplifies visualization greatly
    std::shared_ptr<Area> area;
public:
    
    ExampleWorld(); //the constructor
    
    //evaluate is called by main, hands over a population,
    //and expects each organism in the popolation to have "score" assigned to it's datamap
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    //for ease of use, the base class will call with function on  each organism, as
    //organisms are often evaluated individually
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    
    //these two functions define the number of sensors and motors
    //called when brains are constructed to match to the world
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* TestWorld_hpp */
