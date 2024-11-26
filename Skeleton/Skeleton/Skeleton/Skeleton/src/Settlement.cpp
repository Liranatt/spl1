#pragma once
#include <string>
#include <vector>
#include "../include/Settlement.h"
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type):
        name(name), type(type){};
        const string &getName() const {return name;}
        SettlementType getType() const {return type;}
        const string toString() const {
            return "Settlement name: " + name + ", Settlement type: " + 
           (type == SettlementType::VILLAGE ? "Village" : 
           (type == SettlementType::CITY ? "City" : "Metropolis"));
        }

        private:
            const string name;
            SettlementType type;
};