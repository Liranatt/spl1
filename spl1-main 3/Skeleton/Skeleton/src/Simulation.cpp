#include "../include/Simulation.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
# include "../include/Auxiliary.h"
# include "../include/Action.h"
using std::cout;
using std::endl;
Simulation* backup=nullptr;


Simulation::Simulation(const string &configFilePath):
planCounter(0), isRunning(false), actionsLog(), plans(), settlements(), facilitiesOptions() {
    parse(configFilePath);
};


Simulation::~Simulation() {
    clearData(); 
}

Simulation::Simulation(const Simulation& other)
    : isRunning(other.isRunning), planCounter(other.planCounter),
      facilitiesOptions(other.facilitiesOptions) {
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone()); 
    }
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement)); // check
    }
    for (Plan plan : other.plans) {
        plans.push_back(Plan(plan)); 
    }
}


Simulation& Simulation::operator=(const Simulation& other) {
    if (this != &other) {
        clearData(); 
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }
        for (Plan plan : other.plans) {
            plans.push_back(plan);
        }
    }
    return *this;
}

Simulation::Simulation(Simulation&& other) noexcept:
      isRunning(other.isRunning), planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      facilitiesOptions(std::move(other.facilitiesOptions)),
      settlements(std::move(other.settlements)),
      plans(std::move(other.plans))
       {
    other.clearData();
}

Simulation& Simulation::operator=(Simulation&& other) noexcept {
    if (this != &other) {
        clearData();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = std::move(other.actionsLog);
        facilitiesOptions = std::move(other.facilitiesOptions);
        settlements = std::move(other.settlements);
        plans = std::move(other.plans);

        other.clearData();
    }
    return *this;
}

void Simulation::parse(const std::string &configFilePath) {
    std::fstream file;
    file.open(configFilePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) { 
            std::vector<std::string> arguments = Auxiliary::parseArguments(line);
            
                if (arguments[0] == "settlement") {
                    if(!isSettlementExists(arguments[1])){
                            Settlement* newsettlement = new Settlement(arguments[1], (SettlementType)std::stoi(arguments[2]));//new??
                            addSettlement(newsettlement);               
                    }           
                
                } 

                else if (arguments[0] == "facility") {
                    
                    FacilityType newfacility =  FacilityType(arguments[1], (FacilityCategory)std::stoi(arguments[2]), std::stoi(arguments[3]), std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6]));
                    addFacility(newfacility);
                } 

                else if (arguments[0] == "plan") {
                    if(!isSettlementExists(arguments[1])&&((operator==(arguments[2],"nve"))||(operator==(arguments[2],"bal"))||(operator==(arguments[2],"eco"))&&(operator==(arguments[2],"env")))){
                   
            
                            if(operator==(arguments[2],"nve")){
                                SelectionPolicy* newpolicy = new NaiveSelection();
                                addPlan(getSettlement(arguments[1]),newpolicy);
                            }
                            if(operator==(arguments[2],"bal")){
                                SelectionPolicy* newpolicy = new BalancedSelection(0,0,0);
                                addPlan(getSettlement(arguments[1]),newpolicy);
                            }
                            if(operator==(arguments[2],"eco")){
                                SelectionPolicy* newpolicy = new EconomySelection();
                                addPlan(getSettlement(arguments[1]),newpolicy);
                            }    
                            if(operator==(arguments[2],"env")){
                                SelectionPolicy* newpolicy = new SustainabilitySelection();
                                addPlan(getSettlement(arguments[1]),newpolicy);
                            }
                    }
                }
        file.close();
        }
    }
} 



void Simulation::start(){
    open();
    cout << "the simulation has started" << endl;
    while(isRunning){
        std::string line;
        std::cin>>line;
        vector<string> arguments= Auxiliary::parseArguments(line);
               
            if (arguments[0] == "settlement") {
                AddSettlement action = AddSettlement(arguments[1],(SettlementType)std::stoi(arguments[2]));
                action.act(*this); 
                actionsLog.push_back(&action); 
            }
            if (arguments[0] == "facility") {
                AddFacility action = AddFacility(arguments[1], (FacilityCategory)std::stoi(arguments[2]), std::stoi(arguments[3]), std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6]));
                action.act(*this); 
                actionsLog.push_back(&action);             
            }
            if (arguments[0] == "plan") {
                AddPlan action =  AddPlan(arguments[1], arguments[2]);
                action.act(*this); 
                actionsLog.push_back(&action);
            }

            if (arguments[0]=="step"){
                SimulateStep action =  SimulateStep(std::stoi(arguments[1]));
                action.act(*this); 
                actionsLog.push_back(&action);
            }
            if (arguments[0]=="planStatus"){
                PrintPlanStatus action =  PrintPlanStatus(std::stoi(arguments[1]));
                action.act(*this); 
                actionsLog.push_back(&action);
            }
            if (arguments[0]=="changePolicy"){
                ChangePlanPolicy action =  ChangePlanPolicy(std::stoi(arguments[1]),arguments[2]);
                action.act(*this); 
                actionsLog.push_back(&action);
            }
            
            if (arguments[0]=="backup"){
                BackupSimulation action =  BackupSimulation();
                action.act(*this); 
                actionsLog.push_back(&action);
            }
            if (arguments[0]=="log"){
                PrintActionsLog action =  PrintActionsLog();
                action.act(*this); 
                actionsLog.push_back(&action);
            }
            if (arguments[0]=="restore"){
                RestoreSimulation action =  RestoreSimulation();
                action.act(*this); 
                actionsLog.push_back(&action);
            }
                if (arguments[0]=="close"){
                Close action =  Close();
                action.act(*this); 
                actionsLog.push_back(&action);
            }

    }
    clearData();
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
    settlements.push_back(settlement);
    return true;
}
bool Simulation::addFacility(FacilityType facility){//לתקן
    for (FacilityType Ifacility: facilitiesOptions){
        if (Ifacility.getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}
bool Simulation::isSettlementExists(const string &settlementName) {
    for (Settlement* Isettlement: settlements){
        if(settlementName == Isettlement->getName()){
            return true;
        }
    }
    return false;
}

Settlement& Simulation::getSettlement(const string &settlementName) {
    
    for (Settlement* settle : settlements) {
        if (settle->getName() == settlementName) {
            return *settle; 
        }
    }
Settlement settlement;
return settlement; 
}



bool Simulation::isPlanExists(const int planID) {
    if(planID>=plans.size()){
        return false;
    }
    else{
        return true;
}
}
Plan& Simulation::getPlan(const int planID){
    return plans[planID];

}

void Simulation::step(){
for(Plan stepPlan: plans){
        stepPlan.step();
    }
}
void Simulation::clearData() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    
    for (Settlement* settlement : settlements) {//לבדוק
        delete settlement;
    }
    settlements.clear();
    plans.clear();
    facilitiesOptions.clear();

}
void Simulation::close(){
    for (Plan plan: plans){
        cout <<plan.toStringFClose()<<endl;
    }
    
    isRunning=false;


}
void Simulation::open(){
   isRunning = true;
}

void Simulation::SprintActionLog(){
    for(BaseAction* action: actionsLog){
        cout<< action->toString() <<endl;
    }

}
string Simulation::stringActionLog(){
    string actionLog;
    for(BaseAction* action: actionsLog){
        actionLog+=action->toString()+"\n";
    }
    return actionLog;

}