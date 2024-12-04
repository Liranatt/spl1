#pragma once
#include <vector>
#include "../include/Plan.h"
#include "../include/Facility.h"
#include "../include/Settlement.h"
#include "../include/SelectionPolicy.h"
#include <iostream>
using std::vector;
using std::string;
using namespace std;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
    plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
    life_quality_score(0), economy_score(0), environment_score(0),status(PlanStatus::AVALIABLE){};


Plan::~Plan() {
    clearFacilities(); 
}

    
Plan::Plan(const Plan& other): 
plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(other.selectionPolicy->clone()),
facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),
environment_score(other.environment_score),status(other.status) {
   
    for (Facility* facility : other.facilities) {
        facilities.push_back(&Facility(*facility)); 
    }
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(&Facility(*facility)); 
    }
}

    
    

    
Plan::Plan(Plan&& other) noexcept: 
plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(other.selectionPolicy),facilityOptions(std::move(other.facilityOptions)),
life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score),
status(other.status),facilities(std::move(other.facilities)),underConstruction(std::move(other.underConstruction)) {
    
    other.selectionPolicy = nullptr;
}

    

void Plan:: clearFacilities() {
       
        facilities.clear();
        underConstruction.clear();
        if (selectionPolicy) {
            delete selectionPolicy;
            selectionPolicy = nullptr;
        }
    }




const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan::getEconomyScore() const{
    return economy_score;
}

const int Plan::getEnvironmentScore() const{
    environment_score;
}

string Plan::getSelectionPolicy() const{
    return selectionPolicy->toString(); 
}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
        if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    selectionPolicy = selectionPolicy;
}

void Plan::step() {
    while (status == PlanStatus::AVALIABLE){
        Facility* newF= &Facility(selectionPolicy->selectFacility(facilityOptions),settlement.getName());
        addFacility(newF);
        if (underConstruction.size() >= (int)settlement.getType()+1){
                status = PlanStatus::BUSY;
        }
    }
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        Facility* facility = *it;// להסתכל על הכוכבית 
        int intStatus =(int)facility->step(); 
        if (intStatus == 0) {
            facilities.push_back(facility);
            it = underConstruction.erase(it); 
        }   
        else {
            it++;
        }
        if (underConstruction.size() < (int)settlement.getType()+1){
            status = PlanStatus::AVALIABLE;
        }
        else
            status = PlanStatus::BUSY;
    }
}

void Plan::printStatus(){
    if ((int)status == 0)
    cout<<"AVALIABLE";   
    else
    cout<<"BUSY";         
}

const vector<Facility*> &Plan::getFacilities() const{
    return facilities;
}

void Plan::addFacility(Facility* facility){
    underConstruction.push_back(facility);
}


const string Plan::toString() const {
std::string statusStr = (status == PlanStatus::AVALIABLE) ? "AVALIABLE" : "BUSY";
std::string settlementStr = settlement.toString();

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
        ", Selection Policy: " + getSelectionPolicy() +"\n"+
        ", Life Quality Score: " + std::to_string(life_quality_score) +"\n"+
        ", Economy Score: " + std::to_string(economy_score) +"\n"+
        ", Environment Score: " + std::to_string(environment_score)+"\n"+
        facilitiesPrint +
        underConstructionPrint;
}

const string Plan::toStringFClose() const {
    std::string settlementStr = settlement.toString();
    return 
            "Plan ID: " + std::to_string(plan_id) +"\n"+
            ", Settlement: " + settlementStr +"\n"+
            ", Selection Policy: " + getSelectionPolicy() +"\n"+
            ", Life Quality Score: " + std::to_string(life_quality_score) +"\n"+
            ", Economy Score: " + std::to_string(economy_score) +"\n"+
            ", Environment Score: " + std::to_string(environment_score)+"\n";
}
           
                