#pragma once
#include <string>
#include <vector>
#include "../include/Settlement.h"
using std::string;
using std::vector;

        Settlement::Settlement(const string &name, SettlementType type):
        name(name), type(type){};
        const string &Settlement::getName() const {return name;}
        SettlementType Settlement::getType() const {return type;}
        const string Settlement::toString() const {
            return "Settlement name: " + name + ", Settlement type: " + 
           (type == SettlementType::VILLAGE ? "Village" : 
           (type == SettlementType::CITY ? "City" : "Metropolis"));
        }
