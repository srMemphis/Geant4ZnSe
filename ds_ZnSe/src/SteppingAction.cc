#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleChange.hh"
//#include "GlobalParams.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepStatus.hh"

#include <fstream>

namespace ds_znse
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
 : fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{ 


    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}

