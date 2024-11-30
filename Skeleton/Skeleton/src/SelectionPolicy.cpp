#pragma once
#include <string>
#include <vector>
#include "../include/Facility.h"
#include "../include/SelectionPolicy.h"
using std::string;
using std::vector;
#include <bits/stdc++.h>
using namespace std;

    NaiveSelection::NaiveSelection():
        lastSelectedIndex(-1){};

    const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
        lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
        return facilitiesOptions[lastSelectedIndex];
    }

    const string NaiveSelection::toString() const{
        return "Selection Policy: NaiveSelection";        
    }
    NaiveSelection *clone() const;

    BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
    LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore),
    EnvironmentScore(EnvironmentScore){};
        
    const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
        int fEconomyScore = facilitiesOptions[0].getEconomyScore() + EconomyScore;
        int fEnvironmentScore = facilitiesOptions[0].getEnvironmentScore() + EnvironmentScore;
        int fLifeQualityScore = facilitiesOptions[0].getLifeQualityScore() + LifeQualityScore;
            
        int fmaxdeficit = max(fEconomyScore - fEnvironmentScore,
        fEnvironmentScore - fLifeQualityScore, fEconomyScore - fLifeQualityScore);
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
        const string BalancedSelection::toString() const{
            return "Selection Policy: Balanced Selection";
        }
        BalancedSelection *clone() const override;

        EconomySelection::EconomySelection() {};

        const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
            for (FacilityType eco: facilitiesOptions){
                lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
                if (eco.getCategory() == FacilityCategory::ECONOMY){
                    return eco;
                }
            }
        }
        const string EconomySelection::toString() const{
            return "selectionPolicy: eco";
        }
        EconomySelection *clone() const override;

        SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1) {};
        const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
            for (FacilityType env: facilitiesOptions){
                 lastSelectedIndex= (lastSelectedIndex+1)%facilitiesOptions.size();
                if (env.getCategory() == FacilityCategory::ENVIRONMENT){
                    return env;
                }
            }
        }
        const string SustainabilitySelection::toString() const{
            return "SelectionPolicy: Enviroment";
        }
        SustainabilitySelection *clone() const ;
        // אחר כך ze bishvil haataka habackup