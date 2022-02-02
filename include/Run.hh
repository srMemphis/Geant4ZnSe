#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"

class Run : public G4Run
{
 public:
    Run();
    ~Run();

    void AddCerenkov(G4int);
    void AddScintillation(G4int);
    void Merge(const G4Run*) override;
    void EndOfRun();
    void BeginOfRun();

private:
    G4double fCerenkovCounter;
    G4double fScintillationCounter;
};

#endif