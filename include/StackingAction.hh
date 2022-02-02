#ifndef STACKING_ACTION_HH
#define STACKING_ACTION_HH

#include "G4UserStackingAction.hh"
#include "globals.hh"

class StackingAction : public G4UserStackingAction
{
public:
    StackingAction();
    ~StackingAction();

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
    void NewStage() override;
    void PrepareNewEvent() override;

private:
    G4int fScintillationCounter;
    G4int fCerenkovCounter;
};

#endif
