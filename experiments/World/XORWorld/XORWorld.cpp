#include "XORWorld.hpp"
//#include <cmath>

#ifndef isnan
#define isnan(x) std::isnan(x)
#endif

XorWorld::XorWorld() {


}


// score is number of outputs set to 1 (i.e. output > 0) squared
void XorWorld::evaluateSolo(std::shared_ptr<Organism> organism, bool analyze, bool visualize) {
	//cout << "in test world evalSolo" << endl;
	auto brain = organism->brain;
	double score = 0.0000001;
	int questions[4][2] = { {0,0},{0,1},{1,0},{1,1} };
	double answers[4] = { 0.0,1.0,1.0,0.0 };
	double answer = 0.0;
	for (int tests = 0;tests < 25;tests++) {
		for (int j = 0;j < 4;j++) {
			int i = j;//Random::getIndex(4);
			brain->resetBrain();
			for (int reps = 0;reps < 10;reps++) {
				for (int ins = 0;ins < 2;ins++)
					brain->setInput(ins, questions[i][ins]);
				brain->update();
			}
			answer = brain->getOutput(0);
			if (isnan(answer) || isinf(answer))
				answer = -1.0;

			if (answer > 1.0)
				answer = 1.0;
			if (answer < 0.0)
				answer = 0.0;
			//printf("%f %f\n",answers[i],answer);
			score += 1.0 - ((answers[i] - answer) * (answers[i] - answer));
		}
	}
	organism->dataCollector->setValue("score", score);
}
