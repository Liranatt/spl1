#pragma once
#include <vector>
#include "../include/Facility.h"
#include "../include/Settlement.h"
#include "../include/SelectionPolicy.h"
#include <iostream>
using std::vector;
using namespace std;


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
            if (selectionPolicy != nullptr) {
                delete selectionPolicy;
            }
            selectionPolicy = selectionPolicy;
            
        }
        void step() {
            if (underConstruction.size() < (int)settlement.getType()){
                status = PlanStatus::AVALIABLE;
            }
            else
                status = PlanStatus::BUSY;
            while (status == PlanStatus::AVALIABLE){// צריך לראות האם החזרנו את הטייפ כמו שצריך והאם זה באמת סטרינג
                addFacility(new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName()));
                if (underConstruction.size() < (int)settlement.getType()){
                status = PlanStatus::AVALIABLE;
            }
            else
                status = PlanStatus::BUSY;
            }
            for (auto it = underConstruction.begin(); it != underConstruction.end();) {
                Facility* facility = *it;// להסתכל על הכוכבית 
                facility->step(); 
            
            if ((int)status == 0) {
                facilities.push_back(facility);
                it = underConstruction.erase(it); 
            }   
            else {
                it++;
            }
           if (underConstruction.size() < (int)settlement.getType()){
                status = PlanStatus::AVALIABLE;
            }
            else
                status = PlanStatus::BUSY;
        }
}

        void printStatus(){
             if ((int)status == 0)
             cout<<"Available";   
             else
             cout<<"Busy";         
        }
        const vector<Facility*> &getFacilities() const{
            return facilities;
        }
        void addFacility(Facility* facility){
            underConstruction.push_back(facility);
        }
        const string toString() const;
                // al hazain 
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