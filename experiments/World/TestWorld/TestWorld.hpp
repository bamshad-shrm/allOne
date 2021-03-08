//
//  TestWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef TestWorld_hpp
#define TestWorld_hpp

#include "../World.hpp"

class TestWorld:public World{
public:
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
};


#endif /* TestWorld_hpp */
