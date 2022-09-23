#ifndef ds_znseEventAction_h
#define ds_znseEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

namespace ds_znse
{

class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override;

    void  BeginOfEventAction(const G4Event*) override;
    void    EndOfEventAction(const G4Event*) override;

  private:
    RunAction* fRunAction = nullptr;

};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


