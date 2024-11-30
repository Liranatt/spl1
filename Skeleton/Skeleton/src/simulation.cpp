#include "../include/Simulation.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

       Simulation::Simulation(const string &configFilePath):
            planCounter(0), isRunning(false), actionsLog(), plans(), settlements(), facilitiesOptions() {}

        void Simulation::start(){
            isRunning = true;
            cout << "the simulation has started" << endl;
        }

        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
            plans.push_back(Plan(planCounter++, settlement, selectionPolicy, facilitiesOptions));
        }

        void Simulation::addAction(BaseAction *action){
            actionsLog.push_back(action);
        }

        bool Simulation::addSettlement(Settlement *settlement){
            if (Simulation::isSettlementExists(settlement->getName())){
                return false;
            }
            settlements.push_back(settlement);
            return true;
        }
        bool Simulation::addFacility(FacilityType facility){
            for (FacilityType Ifacility: facilitiesOptions){
                if (Ifacility == facility){
                    return false;
                }
            }
            facilitiesOptions.push_back(facility);
            return true;
        }
        bool Simulation::isSettlementExists(const string &settlementName) {
            for (Settlement Isettlement: settlements){
                if(settlementName == Isettlement.getName()){
                    return true;
                }
            }
            return false;
        }
        Settlement Simulation::&getSettlement(const string &settlementName){
            for (Settlement settle: settlements){
                if (settle.getName() == settlementName)
                    return settle;
            }
            return nullptr;

        }
        Plan& Simulation::getPlan(const int planID){
            return plans[planID];
        }

        void Simulation::step(){
            for(Plan stepplan: plans){
                stepplan.step();
            }
        }

        void Simulation::close(){
            int plannumber = 0;
            for (Plan retplan: plans){
                cout << "plan id: " << (plannumber++) 
                << ", settlementName " << settlements[plannumber]->getName() 
                << ", LifeQuality_Score " << retplan.getlifeQualityScore() < ", Economy_Score: " << retplan.getEconomyScore()
                << ", Enviroment_Score: " << retplan.getEnvironmentScore() <<endl 
            }
        }
        void open();
        void Simulation::SprintActionLog(){
            for(BaseAction action: actionsLog){
                cout<< action.name + " " + action.clone() + " " + action.getStatus();
            }

        }
