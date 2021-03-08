//
//  TheoryOfMindWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/28/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef TheoryOfMindWorld_hpp
#define TheoryOfMindWorld_hpp

#include <memory>
#include <set>
#include "../World.hpp"
#include "../../Core/Population.hpp"
#include "../../Core/Area.hpp"

class TheoryOfMindWorld:public World{
    int xm[4]={0,1,0,-1};
    int ym[4]={-1,0,1,0};
    std::vector<int> targetWorlds;
    std::ofstream analyzeFile,shortAnalyzeFile;
public:
    std::shared_ptr<Area> area;
    TheoryOfMindWorld();
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual double evaluateSoloWorld(std::shared_ptr<Organism> organism,int typeOfWorld,int startCondition, bool analyze,bool visualize);
    virtual int nrOfSensors(void) override {return 6;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 3;}
};


#endif /* TheoryOfMindWorld_hpp */
