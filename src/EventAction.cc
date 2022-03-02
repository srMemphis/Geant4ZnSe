#include "EventAction.hh"
#include "G4Event.hh"

	
EventAction::EventAction():
    G4UserEventAction()
{
} 

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{    
    G4int eveID = event->GetEventID();
    G4cout << "Event № " << eveID << G4endl;

}

void EventAction::EndOfEventAction(const G4Event*)
{
}


