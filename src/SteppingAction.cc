#include "SteppingAction.hh"

#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // G4String name = step->GetPreStepPoint()->GetTouchableHandle()
    //   ->GetVolume()->GetLogicalVolume()->GetName();;

    // if (name == "Detector")
    // {   
    //     G4Track *track = step->GetTrack();
    //     G4String ptype = track->GetDefinition()->GetParticleType();

    //     if (ptype == "opticalphoton") {
    //         G4StepPoint *preStepPoint = step->GetPreStepPoint();

    //         G4ThreeVector photonPos = preStepPoint->GetPosition();
    //         G4double wavelength = 1239.8/(preStepPoint->GetTotalEnergy()/eV);  //in nm

    //         G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    //         analysisManager->FillNtupleDColumn(0, wavelength);
    //         analysisManager->FillNtupleDColumn(1, photonPos[0]);
    //         analysisManager->FillNtupleDColumn(2, photonPos[1]);
    //         analysisManager->FillNtupleDColumn(3, photonPos[2]);
    //         analysisManager->AddNtupleRow(0);
    //         track->SetTrackStatus(fStopAndKill);
    //     }
    // }
}
