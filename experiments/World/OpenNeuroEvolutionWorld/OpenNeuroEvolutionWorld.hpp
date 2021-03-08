//
//  TestWorld.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef OpenNeuroEvolutionWorld_hpp
#define OpenNeuroEvolutionWorld_hpp

#include "../World.hpp"
#include "../../Core/Area.hpp"

class Agent{
public:
    int xPos,yPos,dir,lastFoodEaten;
    int switches[4][4];
    int hits,bites;
    double collected;
    std::shared_ptr<Organism> organism;
    Agent(int startX,int startY,int startDir,std::shared_ptr<Organism> who);
    ~Agent();
};

class OpenNeuroEvolutionWorld:public World{
public:
    int width,height;
    std::shared_ptr<Area> area;
    std::vector<std::vector<std::shared_ptr<Agent>>> whoArea;
    std::vector<std::vector<double>> taskSwitchingCost;
    std::vector<std::vector<std::pair<double,double>>> taskSwitchingCostParams;

    OpenNeuroEvolutionWorld();
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual int nrOfSensors() override;
    virtual int nrOfMotors() override;
    
    virtual void ResetWorld();
    virtual void updateRewardMatrix(double t);

};


#endif /* TestWorld_hpp */
