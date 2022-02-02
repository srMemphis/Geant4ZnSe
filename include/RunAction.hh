#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"

#include "Run.hh"

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    
    Run* fRun;
};

#endif
