#include <string>
#include <vector>
#include "../include/Simulation.h"
#include "../include/Action.h"
#include "../include/Settlement.h"
extern Simulation* backup;

        BaseAction::BaseAction(): errorMsg(), status() {};
        ActionStatus BaseAction::getStatus() const{
            return status;
        }

        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        }

        void BaseAction::error(string errorMsg){
            status = ActionStatus::ERROR;
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
        }
        const string SimulateStep::toString() const{
            return "SimulateStep" + std::to_string(numOfSteps) + " steps";
        }

         SimulateStep* SimulateStep:: clone() const {
            new SimulateStep(numOfSteps);
        } 

        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):// need to add error if name or plan doesnt exist 
        settlementName(settlementName), selectionPolicy(selectionPolicy){};
        void AddPlan::act(Simulation &simulation){
            if(simulation.isSettlementExists(settlementName)&&(!operator==(selectionPolicy,"nve"))&&(!operator==(selectionPolicy,"bal"))
            &&(!operator==(selectionPolicy,"eco"))&&(!operator==(selectionPolicy,"env"))){
                error("Cannot create this plan");
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
            simulation.addAction(this);// האם זה שולח פויינטר?
            
            }
            }
           
        
        

        const string AddPlan::toString() const{
        return "Added plan:" + settlementName + "," + selectionPolicy;
        }
        AddPlan* AddPlan::clone() const {
            return new AddPlan(*this);
        } 

        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
        settlementName(settlementName),settlementType(settlementType){};

        void AddSettlement::act(Simulation &simulation) {
            if(simulation.isSettlementExists(settlementName)){
                error("Settlement already exists");
                } 
            else{
            Settlement* newsettlement = new Settlement(settlementName, settlementType);
            simulation.addSettlement(newsettlement); 
            complete(); 
            }
        simulation.addAction(this);
        }
        AddSettlement* AddSettlement:: clone() const{//לא מצליחה לתקן
            return new AddSettlement(*this);
        }

        const string AddSettlement::toString() const{
                if (BaseAction::getStatus() != ActionStatus::COMPLETED){
                    return getErrorMsg();
                }
                return "settlement: " + settlementName + "Type: " + settlementType + status;
        }



        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
        facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){};

        void AddFacility::act(Simulation &simulation) {
            FacilityType newfacility =  FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
            if(simulation.addFacility(newfacility)){
                complete();
            }
            else{
                error("Facility already exists");
            }
            simulation.addAction(this);
        }

        AddFacility* AddFacility::clone() const {
            return new AddFacility(*this);
        }

        const string AddFacility:: toString() const{
            if (BaseAction::getStatus() != ActionStatus::COMPLETED){
                    return getErrorMsg();
                }
                return "Facility: " + facilityName + facilityCategory;
        }



        PrintPlanStatus::PrintPlanStatus(int planId):
        planId(planId){};

        void PrintPlanStatus::act(Simulation &simulation){
        if(simulation.isPlanExists(planId)){
            (simulation.getPlan(planId)).toString();
        }
        
        else{
            error("Plan doesn't exist");
        }
        
        simulation.addAction(this);//לבדוק אם זה צריך להיות פוינטר או ל
       
        }
        PrintPlanStatus*  PrintPlanStatus:: clone() const {
            new PrintPlanStatus(*this);
        }
        const string toString() const{
            return "planStatus: "; 
        }

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
    planId(planId),newPolicy(newPolicy){};

void ChangePlanPolicy::act(Simulation &simulation) {
    Plan &currentPlan = simulation.getPlan(planId);
    if (currentPlan.selectionPolicy == newPolicy || !simulation.isPlanExists(planID)){
        error("Cannot change selection policy");
    }
    if(operator == (newPolicy,"nve"))
        currentPlan.setSelectionPolicy(new NaiveSelection());
        complete();

    if(operator==(newPolicy,"bal")){         
        int LifeQualityScore=currentPlan.getlifeQualityScore();
        int EconomyScore=currentPlan.getEconomyScore();
        int EnvironmentScore=currentPlan.getEnvironmentScore();
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
    ChangePlanPolicy *clone() const override;
    const string toString() const override;

    PrintActionsLog::PrintActionsLog() {};
    void PrintActionsLog::act(Simulation &simulation){
        simulation.SprintActionLog();
        }
    PrintActionsLog *clone() const override;
    const string toString() const override;

    Close::Close() {};
    void Close::act(Simulation &simulation){
        simulation.close(); 
        }
    Close *clone() const override;
    const string toString() const override;
    
    BackupSimulation::BackupSimulation() {};
    void act(Simulation &simulation){
        simulation.
    }
    BackupSimulation *clone() const override;
    const string toString() const override;

    RestoreSimulation();
    void act(Simulation &simulation) override;
    RestoreSimulation *clone() const override;
    const string toString() const override;