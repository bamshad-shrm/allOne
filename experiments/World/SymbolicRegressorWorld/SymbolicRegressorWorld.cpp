//#include "SymbolicRegressorWorld.h"
#include "SymbolicRegressorWorld.hpp"
#include <limits>
#define PI //3.14159265359
//#include "../../Brain/MarkovBrain/MarkovBrain.h"
#include <cmath>


SymbolicRegressorWorld::SymbolicRegressorWorld() {
    X.clear();
    Y.clear();
    addParamsToX({0.0,0.0},0);
    for(auto params:X){
        Y.push_back(function(params));
    }
}

double SymbolicRegressorWorld::function(vector<double> I) {
	return (I[0] * I[1]) * (I[1] - I[0]);
	//return 8.0/(2.0+(I[0]*I[0])+(I[1]*I[1]));
}

void SymbolicRegressorWorld::addParamsToX(vector<double> params, int where) {

	for (double x = -2.0;x < 2.0;x += 1.0 / 3.0) {
		params[where] = x;
		if (where < params.size() - 1) {
			addParamsToX(params, where + 1);
		}
		else {
			X.push_back(params);
		}
	}
}

// score is number of outputs set to 1 (i.e. output > 0) squared
void SymbolicRegressorWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
	//cout << "in test world evalSolo" << endl;
	auto brain = organism->brain;
	double score = 0.0;
	bool dropDead = false;
	//printf("%s\n",brain->description().c_str());
	//exit(0);
	/*
	for(int x=0;x<(int)X.size();x++){
		for(int brainreps=0;brainreps<10;brainreps++){
			for(int i=0;i<(int)X[x].size();i++)
				brain->setInput(i, X[x][i]);
			brain->update();
		}
		double answer=brain->readOutput(0);
		if((isnan(answer))||(isinf(answer))){
			dropDead=true;
			x=(int)X.size();
		} else {
		score+=(answer-Y[x])*(answer-Y[x]);
		}
	}
	 //*/
	 //*
	double valueMax = 100.0;
	int reps = 100;
	for (int x = 0;x < reps;x++) {
		int whichOne = Random::getIndex((int)X.size());
		brain->resetBrain();
		for (int brainreps = 0;brainreps < 10;brainreps++) {
			for (int i = 0;i < (int)X[whichOne].size();i++) {
				brain->setInput(i, X[whichOne][i]);
			}
			brain->update();
		}
		double answer = brain->getOutput(0);
		//answer=0.0;
		if (abs(answer) > valueMax) {
			answer = valueMax * (answer / abs(answer));
		}
		if ((isnan(answer)) || (isinf(answer))) {
			dropDead = true;
			x = (int)X.size();
		}
		else {
			score += (answer - Y[whichOne]) * (answer - Y[whichOne]);
		}
	}
	//*/
	if (dropDead) {
		organism->dataCollector->setValue("score", 1.0/(1.0+std::numeric_limits<float>::max()));
	}
	else {
		//		score=((valueMax*2.0)*(double)reps)-score;
		organism->dataCollector->setValue("score", 1.0/(1.0+score));//pow(1.00,score/10.0));
	}
	//org->dataMap.Set("gateUsage", brain->description());

	
}
