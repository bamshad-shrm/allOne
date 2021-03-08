//
//  ExampleWorld.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "BladerunnerWorld.hpp"


BladerunnerWorld::BladerunnerWorld(){
    std::vector<std::vector<double>> miniPatterns=std::vector<std::vector<double>>({
        std::vector<double>({1.0, 1.0, 0.0, 0.0}),
        std::vector<double>({0.0, 0.0, 1.0, 1.0}),
        std::vector<double>({1.0, 1.0, 1.0, 0.0}),
        std::vector<double>({0.0, 1.0, 1.0, 1.0}),
        std::vector<double>({-1.0,-1.0, 0.0, 0.0}),
        std::vector<double>({0.0, 0.0,-1.0,-1.0}),
        std::vector<double>({-1.0,-1.0,-1.0, 0.0}),
        std::vector<double>({0.0,-1.0,-1.0,-1.0})
    });
    for(int first=0;first<8;first++)
        for(int second=0;second<8;second++)
            for(int tFirst=0;tFirst<3;tFirst++)
                for(int tSecond=4;tSecond<7;tSecond++){
                    std::vector<std::vector<double>> localPattern;
                    std::vector<std::vector<int>> localWorldState;
                    for(int i=0;i<8;i++){
                        if((i==tFirst)||(i==tFirst+1))
                            localPattern.push_back(miniPatterns[first]);
                        else{
                            if((i==tSecond)||(i==tSecond+1))
                                localPattern.push_back(miniPatterns[second]);
                            else
                                localPattern.push_back(std::vector<double> ({0.0,0.0,0.0,0.0}));
                        }
                        localWorldState.push_back(std::vector<int>({first&1,(first>>1)&1,(first>>2)&1,second&1,(second>>1)&1,(second>>2)&1}));
                    }
                    inputPatterns.push_back(localPattern);
                    worldState.push_back(localWorldState);
                    std::vector<double> localOutput;
                    for(int i=0;i<3;i++){
                        if( ((tFirst>>i)&1)==((tSecond>>i)&1))
                            localOutput.push_back(1.0);
                        else
                            localOutput.push_back(0.0);
                    }
                    targetPatterns.push_back(localOutput);
        }
    /*
    for(auto a:inputPatterns)
        for(auto b:a){
            for(auto c:b)
                std::cout<<c<<" ";
            std::cout<<std::endl;
        }
     */
    Parameters::setParameter("LODManager-parametersToSave",std::string("score,percentage"));
    Parameters::setParameter("GeneticAlgorithm-extraMeans",std::string("percentage"));
    Parameters::setParameter("MarkovBrain-nrOfHidden",9);
    
}

void BladerunnerWorld::evaluate(std::shared_ptr<Population> population,bool analyze,bool visualize){
    if(analyze){
        extraRFile.open(Parameters::getString("BladerunnerWorld-AnalyzeRFile", std::string("Analyze_R.csv")));
        extraRFile<<"ID,R,Rfirst,Rsecond"<<std::endl;
    }
    
    // simple loop iterating over all indididuals of a population
    for(auto organism:population->population){
        evaluateSolo(organism,analyze,visualize);
        extraRFile.flush();
    }
    extraRFile.close();
    
    if(analyze){
        analyzeTransplants(population->population[population->population.size()-1]);
    }
}


void BladerunnerWorld::evaluateSolo(std::shared_ptr<Organism> organism,bool analyze,bool visualize){
    double score=0.0;
    auto brain=organism->brain;     //retrieve the brain from the organism
    double total=0.0;
    double scoreBit=0.0;
    std::vector<std::vector<double>> stateCollector;
    std::vector<double> stateRow;
    for(int p=0;p<(int)inputPatterns.size();p++){
        brain->resetBrain();
        for(int t=0;t<8;t++){
            for(int i=0;i<4;i++)
                brain->setInput(i, inputPatterns[p][t][i]);
            brain->update();
            if(analyze){
                stateRow.clear();
                for(int i=0;i<6;i++){
                    stateRow.push_back((double)worldState[p][t][i]);
                }
                auto h=brain->getHidden();
                stateRow.insert(stateRow.end(),h.begin(),h.end());
                for(int o=0;o<3;o++){
                    stateRow.push_back(brain->getOutput(o));
                }
                stateRow.insert(stateRow.end(),inputPatterns[p][t].begin(),inputPatterns[p][t].end());
                stateCollector.push_back(stateRow);
                //std::cout<<serializeVectorToString(stateRow, ",")<<std::endl;
            }
        }
        for(int o=0;o<3;o++){
            total+=1.0;
            double delta=targetPatterns[p][o]-brain->getOutput(o);
            score+=delta*delta;
            delta=targetPatterns[p][o]-bit(brain->getOutput(o));
            scoreBit+=delta*delta;
            //brain->setOutput(0,0.0);
        }
        
    }
    //std::cout<<score<<" "<<total<<std::endl;
    double percentage=1.0-(double)(score/total);
    score=1.0+percentage;
    //score=score*score;
    percentage=1.0-(double)(scoreBit/total);
    score=score*(1.0+percentage);
    organism->dataCollector->setValue("score",pow(1.05,score*10.0));
    organism->dataCollector->setValue("percentage",percentage);
    if(analyze){
        auto hiddenStateSet=std::vector<int>({6,7,8,9,10,11,12,13,14});
        auto worldStateSet=std::vector<int>({0,1,2,3,4,5});
        auto data=Representations::medianBinarization(stateCollector,std::vector<int>({0,1,2,3,4,5,18,19,20,21}),std::vector<double>({0.5,0.5,0.5,0.5,0.5,0.5,0.0,0.0,0.0,0.0}));
        auto allW=Representations::extractBitPattern(data, worldStateSet);
        auto firstW=Representations::extractBitPattern(data,std::vector<int>({0,1,2}));
        auto secondW=Representations::extractBitPattern(data,std::vector<int>({3,4,5}));
        auto allH=Representations::extractBitPattern(data, hiddenStateSet);
        auto allS=Representations::extractBitPattern(data, std::vector<int>({18,19,20,21}));
        double R=Representations::computeR(allW, allH, allS);
        double Rfirst=Representations::computeR(firstW, allH, allS);
        double Rsecond=Representations::computeR(secondW,allH,allS);
        std::cout<<R<<" "<<Rfirst<<" "<<Rsecond<<std::endl;
        extraRFile<<organism->ID<<","<<R<<","<<Rfirst<<","<<Rsecond<<std::endl;

        std::vector<std::vector<int>> hiddenStatePowerSets;
        for(int i=1;i<(int)pow(2.0,hiddenStateSet.size());i++){
            std::vector<int> subSet;
            for(int j=0;j<(int)hiddenStateSet.size();j++){
                if(((i>>j)&1)==1)
                    subSet.push_back(hiddenStateSet[j]);
            }
            hiddenStatePowerSets.push_back(subSet);
        }
        std::ofstream O;
        O.open(Parameters::getString("BladerunnerWorld-AnalyseFileLead", std::string("AnalyzeEnryptionFile_"))+std::to_string(organism->ID)+".csv");
        O<<"partition,R0,R1,R2,R3,R4,R5"<<std::endl;
        //O<<S;
        for(int b=0;b<(int)hiddenStatePowerSets.size();b++){
            auto HS=Representations::extractBitPattern(data, hiddenStatePowerSets[b]);
            for(int w=0;w<6;w++){
                auto WS=Representations::extractBitPattern(data, std::vector<int>({w}));
                double localR=Representations::computeR(WS, HS, allS);
                if(w==0)
                    O<<"\"["<<serializeVectorToString(hiddenStatePowerSets[b], ",")<<"]\"";
                O<<","<<localR;
                //std::cout<<w<<",\"["<<serializeVectorToString(hiddenStatePowerSets[b], ",")<<"]\","<<localR<<std::endl;
            }
            O<<std::endl;
        }
        O.close();
    }
}

int BladerunnerWorld::nrOfSensors(){
    return 4;
}

int BladerunnerWorld::nrOfMotors(){
    return 3;
}

void BladerunnerWorld::analyzeTransplants(std::shared_ptr<Organism> organism){
    
    std::ofstream O;
    O.open(Parameters::getString("BladerunnerWorld-StateTransplantFile", std::string("StateTransplantFile.csv")));
    O<<"transplantNodeSet,scoreOriginal,scoreTarget"<<std::endl;
    //collect all brainstates in the middle:
    auto brain=organism->brain;     //retrieve the brain from the organism
    std::vector<std::vector<double>> hiddenStateCollector;
    std::vector<std::vector<int>> nodePowerset;
    std::vector<int> nodeList;
    
    for(int p=0;p<(int)inputPatterns.size();p++){
        brain->resetBrain();
        for(int t=0;t<4;t++){
            for(int i=0;i<4;i++)
                brain->setInput(i, inputPatterns[p][t][i]);
            brain->update();
            }
        auto HS=brain->getHidden();
        hiddenStateCollector.push_back(HS);
        //std::cout<<serializeVectorToString(HS, ",")<<std::endl;
    }
    for(int i=0;i<(int)hiddenStateCollector[0].size();i++){
        nodeList.push_back(i);
    }
    for(int i=1;i<(int)pow(2.0,nodeList.size());i++){
        std::vector<int> subSet;
        for(int j=0;j<(int)nodeList.size();j++){
            if(((i>>j)&1)==1)
                subSet.push_back(nodeList[j]);
        }
        nodePowerset.push_back(subSet);
    }
    for(auto transplantNodes:nodePowerset){
        double total=0.0;
        double scoreOriginal=0.0;
        double scoreTarget=0.0;
        for(int originalPattern=0;originalPattern<(int)inputPatterns.size();originalPattern++){
            for(int targetPattern=0;targetPattern<(int)inputPatterns.size();targetPattern++){
                brain->resetBrain();
                for(int t=0;t<8;t++){
                    for(int i=0;i<4;i++){
                        brain->setInput(i, inputPatterns[originalPattern][t][i]);
                    }
                    if(t==4){
                        for(auto node:transplantNodes)
                            brain->setHidden(node, hiddenStateCollector[targetPattern][node]);
                    }
                    brain->update();
                }
                for(int o=0;o<3;o++){
                    double deltaOriginal=targetPatterns[originalPattern][o]-bit(brain->getOutput(o));
                    scoreOriginal+=deltaOriginal*deltaOriginal;
                    double deltaTarget=targetPatterns[targetPattern][o]-bit(brain->getOutput(o));
                    scoreTarget+=deltaTarget*deltaTarget;
                    total+=1.0;
                }
            }
        }
        std::cout<<"\"["<<serializeVectorToString(transplantNodes, ",")<<"]\","<<scoreOriginal/total<<","<<scoreTarget/total<<std::endl;
        O<<"\"["<<serializeVectorToString(transplantNodes, ",")<<"]\","<<scoreOriginal/total<<","<<scoreTarget/total<<std::endl;

    }
    O.close();
}


/*
*/
//
