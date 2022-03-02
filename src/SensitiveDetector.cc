#include "SensitiveDetector.hh"
#include "EventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AccumulableManager.hh"
#include "G4EventManager.hh"

SensitiveDetector::SensitiveDetector(G4String name):
    G4VSensitiveDetector(name)
{  
    analysisManager = G4AnalysisManager::Instance();
    fSignalAmplitude = 0;
}

SensitiveDetector::~SensitiveDetector()
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   

    G4Track *track = aStep->GetTrack();
    G4String ptype = track->GetDefinition()->GetParticleType();

    if (ptype == "opticalphoton") {
        G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

        G4ThreeVector photonPos = preStepPoint->GetPosition();
        G4double wavelength = 1239.8/(preStepPoint->GetTotalEnergy()/eV);  //in nm

        analysisManager->FillNtupleDColumn(0, 0, wavelength);
        analysisManager->FillNtupleDColumn(0, 1, photonPos[0]);
        analysisManager->FillNtupleDColumn(0, 2, photonPos[1]);
        analysisManager->FillNtupleDColumn(0, 3, photonPos[2]);
        analysisManager->AddNtupleRow(0);
        fSignalAmplitude += 1;
        track->SetTrackStatus(fStopAndKill);
    }
    return true;
}


void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fSignalAmplitude = 0;
}
void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    // G4cout << "Signal Amplitude:" << fSignalAmplitude << G4endl;
    analysisManager->FillNtupleDColumn(1, 0, fSignalAmplitude);
    analysisManager->AddNtupleRow(1);
}
