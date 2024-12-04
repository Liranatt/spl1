#include <string>
#include <vector>
#include "../include/Simulation.h"
#include "../include/Action.h"
#include "../include/Settlement.h"
#include <iostream>
using std::cout;
using std::endl;
extern Simulation* backup;

        BaseAction::BaseAction(): errorMsg(), status(ActionStatus::ERROR) {};
        ActionStatus BaseAction::getStatus() const{
            return status;
        }
        
        string BaseAction:: stringStatus()const{
            if(getStatus()==ActionStatus::COMPLETED){
                return "COMPLETED";
            }
            else
                return "ERROR";
            
        }
        
        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        }

        void BaseAction::error(string errorMsg){
            errorMsg = "Error: <" + errorMsg + ">";
        }
        const string &BaseAction::getErrorMsg() const{
            return errorMsg;
        }





        SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}
        void SimulateStep::act(Simulation &simulation){
            for (int i = 0; i < numOfSteps; i++){
                simulation.step();
            }
            complete();
        }
        const string SimulateStep::toString() const{

            return "step " + std::to_string(numOfSteps) +" "+ stringStatus();
        }

         SimulateStep* SimulateStep:: clone() const {
            return new SimulateStep(*this);
        } 





        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):// need to add error if name or plan doesnt exist 
        settlementName(settlementName), selectionPolicy(selectionPolicy){error("Cannot create this plan");};
        
        void AddPlan::act(Simulation &simulation){
            if(simulation.isSettlementExists(settlementName)&&(!operator==(selectionPolicy,"nve"))&&(!operator==(selectionPolicy,"bal"))
            &&(!operator==(selectionPolicy,"eco"))&&(!operator==(selectionPolicy,"env"))){
                    std::cout << getErrorMsg() << std::endl;
            }
            else{
                    if(operator==(selectionPolicy,"nve")){
                        SelectionPolicy* newpolicy = new NaiveSelection();
                        simulation.addPlan(simulation.getSettlement(settlementName),newpolicy);
                    }
                    if(operator==(selectionPolicy,"bal")){
                        SelectionPolicy* newpolicy = new BalancedSelection(0,0,0);
                        simulation.addPlan(simulation.getSettlement(settlementName),newpolicy);
                    }
                    if(operator==(selectionPolicy,"eco")){
                        SelectionPolicy* newpolicy = new EconomySelection();
                        simulation.addPlan(simulation.getSettlement(settlementName),newpolicy);
                    }    
                    if(operator==(selectionPolicy,"env")){
                        SelectionPolicy* newpolicy = new SustainabilitySelection();
                        simulation.addPlan(simulation.getSettlement(settlementName),newpolicy);
                    }
                    
                    complete();
            
            }
            
            }
           
        const string AddPlan::toString() const{
        return "plan" + settlementName + " " + selectionPolicy;
        }
        AddPlan* AddPlan::clone() const {
            return new AddPlan(*this);
        } 





        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
        settlementName(settlementName),settlementType(settlementType){error("Settlement already exists");};

        void AddSettlement::act(Simulation &simulation) {
            if(simulation.isSettlementExists(settlementName)){
                std::cout << getErrorMsg() << std::endl;
                } 
            else{
            Settlement* newsettlement = new Settlement(settlementName, settlementType);//new??
            simulation.addSettlement(newsettlement); 
            complete(); 
            }
       
        }
        AddSettlement* AddSettlement:: clone() const{
            return new AddSettlement(*this);
        }

        const string AddSettlement::toString() const{
             int intST=(int)settlementType; 
             string  stringST= std::to_string(intST);
                
            return "settlement " + settlementName + " " + stringST + stringStatus();
        }



        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
        facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){error("Facility already exists");};

        void AddFacility::act(Simulation &simulation) {
            FacilityType newfacility =  FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
            if(simulation.addFacility(newfacility)){
                complete();
            }
            else{
               std::cout << getErrorMsg() << std::endl;
            }
           
        }

        AddFacility* AddFacility::clone() const {
            return new AddFacility(*this);
        }

        const string AddFacility:: toString() const{
            int intFC=(int)facilityCategory; 
            string  stringFC= std::to_string(intFC);
                
                return "facility " + facilityName +" "+ stringFC+" "+ std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+" "+stringStatus();
        
        }




        PrintPlanStatus::PrintPlanStatus(int planId):
        planId(planId)
            {
            error("Plan doesn't exist");
            };

        void PrintPlanStatus::act(Simulation &simulation){
            
            if(simulation.isPlanExists(planId)){
                Plan& plan = simulation.getPlan(planId); // Get the plan by reference
                std::cout << plan.toString() << std::endl;
                complete();
            }

            else{
                std::cout << getErrorMsg() << std::endl;
            }       
        }

        PrintPlanStatus*  PrintPlanStatus:: clone() const {
            new PrintPlanStatus(*this);
        }

        const string PrintPlanStatus::toString() const{
            
            return "planStatus "+ std::to_string(planId)+" "+stringStatus(); 

        }






        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
            planId(planId),newPolicy(newPolicy){error("Cannot change selection policy");};

        void ChangePlanPolicy::act(Simulation &simulation) {
            Plan &currentPlan = simulation.getPlan(planId);
                   
                    if (currentPlan.getSelectionPolicy() == newPolicy || !simulation.isPlanExists(planId)){
                        std::cout << getErrorMsg() << std::endl;
                    }
                    if(operator == (newPolicy,"nve"))
                        currentPlan.setSelectionPolicy(new NaiveSelection());
                        complete();

                    if(operator==(newPolicy,"bal")){         
                        int LifeQualityScore= currentPlan.getlifeQualityScore();
                        int EconomyScore= currentPlan.getEconomyScore();
                        int EnvironmentScore= currentPlan.getEnvironmentScore();
                        currentPlan.setSelectionPolicy(new BalancedSelection(LifeQualityScore,EconomyScore,EnvironmentScore));
                        complete();
                    }

                    if(operator == (newPolicy,"eco")){
                        currentPlan.setSelectionPolicy(new EconomySelection());
                        complete();
                    }
                    if(operator == (newPolicy,"env")){
                        currentPlan.setSelectionPolicy(new SustainabilitySelection());
                        complete();    
                    }

        }

        ChangePlanPolicy* ChangePlanPolicy::clone() const {
            return new ChangePlanPolicy(*this);
        }
        
        const string ChangePlanPolicy::toString() const {
            return "changePolicy "+std::to_string(planId)+" "+newPolicy+" "+stringStatus();
        }





        PrintActionsLog::PrintActionsLog() {};
        void PrintActionsLog::act(Simulation &simulation){
            
            simulation.SprintActionLog();
            complete();
        
        }

        PrintActionsLog *PrintActionsLog::clone() const {
            return new PrintActionsLog(*this);
        }

        const string PrintActionsLog::toString() const {
            return "log "+stringStatus();
        }




        Close::Close() {};
        void Close::act(Simulation &simulation){
            simulation.close(); 
            }
        Close *Close::clone() const {};
        const string Close::toString() const {};
    




    BackupSimulation::BackupSimulation() {};
    void BackupSimulation::act(Simulation &simulation) {
    if (backup != nullptr) {
        delete backup; 
    }
    backup->simulation; 
    complete(); 
    }
    BackupSimulation *BackupSimulation:: clone() const {
        return new BackupSimulation(*this);
    }
    const string BackupSimulation::toString() const {
        return "backup"+ stringStatus();
    }

   
   
    RestoreSimulation::RestoreSimulation(){error("No backup available");};
    void RestoreSimulation::act(Simulation &simulation) {
        if (backup == nullptr) {
            std::cout << getErrorMsg() << std::endl;
        }
        else{
            simulation.clearData();
            simulation = *backup; 
            complete();
        }
    }

    RestoreSimulation* RestoreSimulation::clone() const {
        return new RestoreSimulation(*this);
    }

    const string RestoreSimulation::toString() const {
        return "restore " + stringStatus();
    }
