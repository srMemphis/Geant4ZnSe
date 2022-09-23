#include "SensitiveDetector.hh"
#include "EventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AccumulableManager.hh"
#include "G4RunManager.hh"

namespace ds_znse
{
SensitiveDetector::SensitiveDetector(G4String name):
    G4VSensitiveDetector(name)
{  
}

SensitiveDetector::~SensitiveDetector()
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{   
    auto track = step->GetTrack();
    if (track->GetParentID() >=0) {
        G4String ptype = track->GetDefinition()->GetParticleType();
        if (ptype == "opticalphoton") {
            auto pos = step->GetPreStepPoint()->GetPosition();
            G4double ang = step->GetPreStepPoint()->GetMomentumDirection().angle(G4ThreeVector(0,0,1));
            G4String proc_name = track->GetCreatorProcess()->GetProcessName();

            analManager->FillNtupleDColumn(0, 0, ang);
            analManager->FillNtupleDColumn(0, 1, pos.getX());
            analManager->FillNtupleDColumn(0, 2, pos.getY());
            analManager->FillNtupleDColumn(0, 3, pos.getZ());
            analManager->FillNtupleDColumn(0, 4,(track->GetKineticEnergy()/eV));
            analManager->FillNtupleSColumn(0, 5, proc_name);
            analManager->FillNtupleDColumn(0, 6, nEvent);
            analManager->AddNtupleRow(0); 

            fSignalAmplitude += 1;
            fSignAmp1 += 1 * track->GetKineticEnergy()/eV;

            track->SetTrackStatus(fStopAndKill);
        }
 
    }   
    return true;
}


void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    nEvent = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    analManager = G4AnalysisManager::Instance();
    fSignalAmplitude = 0;
    fSignAmp1 = 0;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    analManager->FillNtupleDColumn(1, 0, fSignalAmplitude);
    analManager->FillNtupleDColumn(1, 1, fSignAmp1);
    analManager->AddNtupleRow(1);

}

}
