#include "Run.hh"

Run::Run():
    G4Run()
{
    fCerenkovCounter = 0;
    fScintillationCounter = 0;
}

Run::~Run()
{
}

void Run::AddCerenkov(G4int n)
{
    fCerenkovCounter += n;
}

void Run::AddScintillation(G4int n)
{
    fScintillationCounter += n;
}

void Run::Merge(const G4Run* run)
{
    const Run* localRun = static_cast<const Run*>(run);

    fCerenkovCounter += localRun->fCerenkovCounter;
    fScintillationCounter += localRun->fScintillationCounter;

    G4Run::Merge(run);
}

void Run::EndOfRun()
{
    G4cout << "----------------------------------- END OF RUN --------------------------------" << G4endl;
    G4cout << "Total Scintillation count: " << fScintillationCounter << G4endl;
    G4cout << "Total Cherenkov count: " << fCerenkovCounter << G4endl;
}










