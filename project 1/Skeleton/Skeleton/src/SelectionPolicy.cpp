#pragma once
#include <string>
#include <vector>
#include "../include/Facility.h"
#include "../include/SelectionPolicy.h"
using std::string;
using std::vector;
#include <bits/stdc++.h>
using namespace std;
class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection(){
            lastSelectedIndex = -1;
        }
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override {
            lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
            return facilitiesOptions[lastSelectedIndex];
        }

        const string toString() const override {
            //int cosemashelachem;
           // return cosemashelachem;
            ;
        }
        NaiveSelection *clone() const override;
        ~NaiveSelection() override = default;
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
        LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){};
        
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override {
            int fEconomyScore = facilitiesOptions[0].getEconomyScore() + EconomyScore;
            int fEnvironmentScore = facilitiesOptions[0].getEnvironmentScore() + EnvironmentScore;
            int fLifeQualityScore = facilitiesOptions[0].getLifeQualityScore() + LifeQualityScore;
            
            int fmaxdeficit = max(fEconomyScore - fEnvironmentScore, fEnvironmentScore - fLifeQualityScore, fEconomyScore - fLifeQualityScore);
            int i = 0;
            int retind = 0;
            for(FacilityType index: facilitiesOptions){
                int tempeco = EconomyScore + index.getEconomyScore();
                int tempenv = EnvironmentScore + index.getEnvironmentScore();
                int tempbal = LifeQualityScore + index.getLifeQualityScore();
                int maxdeficit = max(tempeco - tempenv, tempenv - tempbal, tempeco - tempbal);
                if(maxdeficit < fmaxdeficit){
                    fmaxdeficit = maxdeficit;
                    retind = i;
                    }
                i++;
              }

              return facilitiesOptions[retind];   
        }
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override{
             for (FacilityType eco: facilitiesOptions){
                 lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
                if (eco.getCategory() == FacilityCategory::ECONOMY){
                    return eco;
                }
            }
        }
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override{
            for (FacilityType env: facilitiesOptions){
                 lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
                if (env.getCategory() == FacilityCategory::ENVIRONMENT){
                    return env;
                }
            }
        }
        const string toString() const override;
    // אחר כך 
        SustainabilitySelection *clone() const override;
        // אחר כך ze bishvil haataka habackup
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};