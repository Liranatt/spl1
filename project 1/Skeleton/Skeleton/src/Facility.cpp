#pragma once
#include <string>
#include <vector>
#include "../include/Facility.h"
using std::string;
using std::vector;

enum class FacilityStatus {
    UNDER_CONSTRUCTIONS,
    OPERATIONAL,
};

enum class FacilityCategory {
    LIFE_QUALITY,
    ECONOMY,
    ENVIRONMENT,
};


class FacilityType {
    public:
    

        FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
        name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){};   
        
        const string &getName() const {return name;}
        int getCost() const {return price;}
        int getLifeQualityScore() const {return lifeQuality_score;}
        int getEnvironmentScore() const {return environment_score;}
        int getEconomyScore() const {return economy_score;}
        FacilityCategory getCategory() const {return category;}

        protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
       

    
};



class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
        FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score) ,settlementName(settlementName), status(FacilityStatus:: UNDER_CONSTRUCTIONS), timeLeft(price){};
        
        Facility(const FacilityType &type, const string &settlementName):

        FacilityType(type.getName(), type.getCategory(), type.getCost(), type.getLifeQualityScore(), type.getEconomyScore(), type.getEnvironmentScore()), 
        settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}
               
        const string &getSettlementName() const {return settlementName;}
        const int getTimeLeft() const {return timeLeft;}
        FacilityStatus step(){
            timeLeft--;
            if (timeLeft == 0){
                setStatus(FacilityStatus::OPERATIONAL);
                return status;
            }
            setStatus(FacilityStatus::UNDER_CONSTRUCTIONS);
            return FacilityStatus::UNDER_CONSTRUCTIONS;
        } 
        void setStatus(FacilityStatus status){
            status = status;
        }
        const FacilityStatus& getStatus() const {return status;}
        const string toString() const {
        return "Facility: " + getName() + ", Settlement: " + settlementName +
               ", Status: " + (status == FacilityStatus::OPERATIONAL ? "Operational" : "Under Construction") +
               ", Time Left: " + std::to_string(timeLeft);
    }
    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};