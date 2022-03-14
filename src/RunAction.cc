#include "RunAction.hh"

#include "G4AnalysisManager.hh"

RunAction::RunAction():
    G4UserRunAction(),
    fRun(nullptr)
{

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleDColumn("Wavelength");
    analysisManager->CreateNtupleDColumn("X");
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("Amplitude", "Amplitude");
    analysisManager->CreateNtupleDColumn("Amplitude");
    analysisManager->FinishNtuple(1);

}

RunAction::~RunAction()
{
}

G4Run* RunAction::GenerateRun()
{
    fRun = new Run();
    return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("4.00.root");
}

void RunAction::EndOfRunAction(const G4Run*)
{   
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    if(isMaster)
    {
        fRun->EndOfRun();
    }
    analysisManager->Write();
    analysisManager->CloseFile();

}
