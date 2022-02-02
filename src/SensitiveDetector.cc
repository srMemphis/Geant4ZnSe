#include "SensitiveDetector.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(G4String name):
    G4VSensitiveDetector(name)
{  
    // G4cout << "SENSITIVE DETECTOR CONSTRUCTOR" << G4endl;

}

SensitiveDetector::~SensitiveDetector()
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   

    // G4cout << "SENSITIVE DETECTOR HIT!" << G4endl;
    G4Track *track = aStep->GetTrack();
    G4String ptype = track->GetDefinition()->GetParticleType();

    if (ptype == "opticalphoton") {
        G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

        G4ThreeVector photonPos = preStepPoint->GetPosition();
        G4double wavelength = 1239.8/(preStepPoint->GetTotalEnergy()/eV);  //in nm

        G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleDColumn(0, wavelength);
        analysisManager->FillNtupleDColumn(1, photonPos[0]);
        analysisManager->FillNtupleDColumn(2, photonPos[1]);
        analysisManager->FillNtupleDColumn(3, photonPos[2]);
        analysisManager->AddNtupleRow(0);
        track->SetTrackStatus(fStopAndKill);
    }
    return true;
}


void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    // G4cout << "SENSITIVE DETECTOR INITIALIZE" << G4endl;
}
void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    // G4cout << "SENSITIVE DETECTOR END OF EVENT" << G4endl;
}
