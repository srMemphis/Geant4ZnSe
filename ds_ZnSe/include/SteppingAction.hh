#ifndef ds_znseSteppingAction_h
#define ds_znseSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

namespace ds_znse
{

class EventAction;

/// Event action class
///
class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step*) override;

  private:
    EventAction* fEventAction = nullptr;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


