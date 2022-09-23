#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"
#include "G4AccumulableManager.hh"
#include "G4Timer.hh"

namespace ds_znse
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  G4AnalysisManager *analManager = G4AnalysisManager::Instance();
  analManager->SetNtupleMerging(true);

  analManager->CreateNtuple("Photons", "Photons");
  analManager->CreateNtupleDColumn("angle");
  analManager->CreateNtupleDColumn("X");
  analManager->CreateNtupleDColumn("Y");
  analManager->CreateNtupleDColumn("Z");
  analManager->CreateNtupleDColumn("Energy");
  analManager->CreateNtupleSColumn("ProcessName");
  analManager->CreateNtupleDColumn("nEvent");
  analManager->FinishNtuple(0);

  analManager->CreateNtuple("Amplitude", "Amplitude");
  analManager->CreateNtupleDColumn("Counts");
  analManager->CreateNtupleDColumn("Countsenergy");
  analManager->FinishNtuple(1);

  analManager->CreateNtuple("Primaries", "Primaries");
  analManager->CreateNtupleDColumn("Energy");
  analManager->FinishNtuple(2);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4AnalysisManager *analManager = G4AnalysisManager::Instance();
  auto timer = new G4Timer();
  G4String cursystime = timer->GetClockTime();
  analManager->OpenFile("out " + cursystime + ".root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  G4AnalysisManager *analManager = G4AnalysisManager::Instance();
  analManager->Write();
  analManager->CloseFile();

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
    = static_cast<const PrimaryGeneratorAction*>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String partName;
  if (generatorAction)
  {
    G4ParticleDefinition* particle
      = generatorAction->GetParticleGun()->GetParticleDefinition();
    partName = particle->GetParticleName();
  }

  // Print results
  //
  if (IsMaster())
  {
    G4cout  << G4endl << "--------------------End of Global Run-----------------------" << G4endl
    << "  The run was " << nofEvents << " events " << G4endl;
     
  }
  else
  {
    // G4cout
    //  << G4endl
    //  << "--------------------End of Local Run------------------------"
    //  << G4endl
    //  << "  The run was " << nofEvents << " "<< partName << G4endl;
  }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


}//namespace end

