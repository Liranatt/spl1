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
        plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
        life_quality_score(0), economy_score(0), environment_score(0),status(PlanStatus::AVALIABLE){};

    
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
        while (status == PlanStatus::AVALIABLE){// צריך לראות האם החזרנו את הטייפ כמו שצריך והאם זה באמת סטרינג
            Facility* newF= new Facility(SelectionPolicy::selectionPolicy->selectFacility(facilityOptions),Settlement::settlement.getName());
            addFacility(newF);
            if (underConstruction.size() >= (int)settlement.getType()+1){
                  status = PlanStatus::BUSY;
            }
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


    const string Plan::toString() const {
    std::string statusStr = (status == PlanStatus::AVALIABLE) ? "Available" : "Busy";
    std::string settlementStr = settlement.toString();
    std::string selectionPolicyStr = (selectionPolicy != nullptr) ? selectionPolicy->toString() : "No Selection Policy";
    std::string facilitiesPrint;
    for (Facility* fac : facilities) {
        facilitiesPrint +=fac->toString() + "\n";
    }

    std::string underConstructionPrint;
    for (Facility* fac : underConstruction) {
        underConstructionPrint +=fac->toString() + "\n";
    }
           
    return "Plan ID: " + std::to_string(plan_id) +"\n"+
           ", Settlement: " + settlementStr +"\n"+
           ", Status: " + statusStr +"\n"+
           ", Selection Policy: " + selectionPolicyStr +
           ", Life Quality Score: " + std::to_string(life_quality_score) +"\n"+
           ", Economy Score: " + std::to_string(economy_score) +"\n"+
           ", Environment Score: " + std::to_string(environment_score)+"\n"+
           facilitiesPrint +
           underConstructionPrint;
}
                 