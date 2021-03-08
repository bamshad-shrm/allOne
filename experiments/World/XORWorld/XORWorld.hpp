


#pragma once

#include "../World.hpp"



#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class XorWorld : public World {
public:

    XorWorld();

    
    virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;

    
    virtual int nrOfSensors(void) { return 2; } //force this -> JS?
    virtual int nrOfMotors(void) { return 1; }

};
