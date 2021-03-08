//
//  EdlundMaze.hpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 10/3/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#ifndef EdlundMazeWorld_hpp
#define EdlundMazeWorld_hpp

#include <memory>
#include "../World.hpp"
#include "../../Core/Population.hpp"
#include "../../Core/Area.hpp"

class EdlundMazeWorld:public World{
public:
    std::shared_ptr<Area> maze;
    std::vector<int> doorPlacement;
    
    virtual void evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize) override;
    virtual void evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize) override;
    
    EdlundMazeWorld(){ makeNewMaze();}
    
    void makeNewMaze();
    void showMaze();
    virtual int nrOfSensors(void) override {return 6;} //force this -> JS?
    virtual int nrOfMotors(void) override {return 2;}
    
};

#endif /* EdlundMazeWorld_hpp */
