#pragma once
#include <vector>
#include "../include/Plan.h"
#include "../include/Facility.h"
#include "../include/Settlement.h"
#include "../include/SelectionPolicy.h"
#include <iostream>
using std::vector;
using namespace std;

    Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
        plan_id(planId), settlement(&settlement), 
        selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
        life_quality_score(0), economy_score(0), environment_score(0){};

    const int Plan::getlifeQualityScore() const{
        return life_quality_score;
    }

    const int Plan::getEconomyScore() const{
        return economy_score;
    }

    const int Plan::getEnvironmentScore() const{
        environment_score;
    }

    void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
         if (selectionPolicy != nullptr) {
            delete selectionPolicy;
        }
        selectionPolicy = selectionPolicy;
    }

    void Plan::step() {
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

    void Plan::printStatus(){
        if ((int)status == 0)
        cout<<"Available";   
        else
        cout<<"Busy";         
    }

    const vector<Facility*> &Plan::getFacilities() const{
        return facilities;
    }

    void Plan::addFacility(Facility* facility){
        underConstruction.push_back(facility);
    }
        const string toString() const;
                // al hazain 