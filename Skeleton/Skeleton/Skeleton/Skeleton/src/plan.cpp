#pragma once
#include <vector>
#include "../include/Facility.h"
#include "../include/Settlement.h"
#include "../include/SelectionPolicy.h"
using std::vector;


enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
        plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),life_quality_score(0), economy_score(0), environment_score(0){};
        const int getlifeQualityScore() const{
            return life_quality_score;
        }
        const int getEconomyScore() const{
            return economy_score;
        }
        const int getEnvironmentScore() const{
            environment_score;
        }
        void setSelectionPolicy(SelectionPolicy *selectionPolicy){
            if (selectionPolicy == "nve")
            {
                underConstruction[SelectionPolicy::selectFacility(facilityOptions)];
            }
            
        }
        void step();
        void printStatus(){
            cout<<
        }
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};