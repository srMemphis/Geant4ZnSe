#include "StackingAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

#include "Run.hh"

StackingAction::StackingAction():
    G4UserStackingAction(),
    fScintillationCounter(0),
    fCerenkovCounter(0)
{
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
    if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) // particle is optical photon
    {  
        if(aTrack->GetParentID() > 0) // particle is secondary
        {  
            if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")
            {
                ++fScintillationCounter;
                // G4cout << "Scintillation! total: " << fScintillationCounter << G4endl;
            }
            else if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
            {
                ++fCerenkovCounter;
                // G4cout << "Cherenok! total: " << fCerenkovCounter << G4endl;
                //return fKill;  //kill this motherfucker
            }
    }
  }
  //urgent stack
  return fUrgent;
}

void StackingAction::NewStage()
{
    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    run->AddScintillation((G4double) fScintillationCounter);
    run->AddCerenkov((G4double) fCerenkovCounter);
}

void StackingAction::PrepareNewEvent()
{
    fScintillationCounter = 0;
    fCerenkovCounter = 0;
}
