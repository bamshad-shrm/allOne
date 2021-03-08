#pragma once

#include "../World.hpp"

#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class SymbolicRegressorWorld : public World {

public:

	SymbolicRegressorWorld();
	
	virtual ~SymbolicRegressorWorld() = default;

	void addParamsToX(vector<double> params, int where);
	double function(vector<double> I);
	vector<vector<double>> X;
	vector<double> Y;

	virtual void evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) override;

    virtual int nrOfSensors(void) override { return 4; }//force this -> JS?
    virtual int nrOfMotors(void) override { return 1; }

};

