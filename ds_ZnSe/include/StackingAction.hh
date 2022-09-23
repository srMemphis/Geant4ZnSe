
#ifndef ds_znseStackingAction_h
#define ds_znseStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"


namespace ds_znse
{

class EventAction;

/// Stacking action class : manage the newly generated particles
///

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction(EventAction* eventAction);
    ~StackingAction() override;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;

  private:
  EventAction* fEventAction = nullptr;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

