//
//  DataCollector.cpp
//  NoCompromiseMABE
//
//  Created by ArendHintze on 9/14/19.
//  Copyright © 2019 ArendHintze. All rights reserved.
//

#include "DataCollector.hpp"
void DataCollector::setValue(std::string key, std::string value){
    stringData[key]=value;
}

void DataCollector::setValue(std::string key, int value){
    intData[key]=value;
}

void DataCollector::setValue(std::string key, double value){
    doubleData[key]=value;
}

void DataCollector::setValue(std::string key, std::vector<double> value){
    doubleVectorData[key]=value;
}

void DataCollector::setValue(std::string key, std::vector<int> value){
    intVectorData[key]=value;
}

void DataCollector::addValue(std::string key, int value){
    if(intData.find(key)==intData.end())
        setValue(key,value);
    else
        intData[key]+=value;
}

void DataCollector::addValue(std::string key, double value){
    if(doubleData.find(key)==doubleData.end())
        doubleData[key]=value;
    else
        doubleData[key]+=value;

}

void DataCollector::multValue(std::string key, int value){
    if(intData.find(key)==intData.end())
        setValue(key,value);
    else
        intData[key]*=value;
}

void DataCollector::multValue(std::string key, double value){
    if(doubleData.find(key)==doubleData.end())
        doubleData[key]=value;
    else
        doubleData[key]*=value;
}

void DataCollector::appendValue(std::string key, int value){
    if(stringData.find(key)==stringData.end())
        stringData[key]="\"["+std::to_string(value)+"]\"";
    else
        stringData[key].insert(2,std::to_string(value)+",");
}

void DataCollector::appendValue(std::string key, double value){
    if(stringData.find(key)==stringData.end())
        stringData[key]="\"["+std::to_string(value)+"]\"";
    else
        stringData[key].insert(2,std::to_string(value)+",");
}

void DataCollector::appendValue(std::string key, std::string value){
    if(stringData.find(key)==stringData.end())
        stringData[key]="\"["+value+"]\"";
    else
        stringData[key].insert(2,value+",");
}

std::string DataCollector::getAsStringFromUnknownType(std::string key){
    if(stringData.find(key)!=stringData.end())
        return stringData[key];
    if(intData.find(key)!=intData.end())
        return std::to_string(intData[key]);
    if(doubleData.find(key)!=doubleData.end())
        return std::to_string(doubleData[key]);
    if(doubleVectorData.find(key)!=doubleVectorData.end())
        return "\"["+serializeVectorToString(doubleVectorData[key],",")+"]\"";
    if(intVectorData.find(key)!=intVectorData.end())
        return "\"["+serializeVectorToString(intVectorData[key],",")+"]\"";

    std::cout<<"you querried DataCollector for a Key that does not exist: "<<key<<std::endl;
    exit(0);    
}

std::vector<std::string> DataCollector::getAllKeysDouble(){
    std::vector<std::string> keys;
    for(std::map<std::string,double>::iterator it = doubleData.begin(); it != doubleData.end(); ++it)
        keys.push_back(it->first);
    return keys;
}
