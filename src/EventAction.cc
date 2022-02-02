#include "EventAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
	
EventAction::EventAction():
    G4UserEventAction()
{
    man = G4RunManager::GetRunManager();
} 

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{    
    G4int runID = man->GetCurrentRun()->GetRunID();
    G4int eveID = event->GetEventID();
    G4cout << "Run № " << runID << ", event № " << eveID << G4endl;
}

void EventAction::EndOfEventAction(const G4Event*)
{  
}
