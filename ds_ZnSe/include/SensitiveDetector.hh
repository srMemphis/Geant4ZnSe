#ifndef ds_znsesensitive_detector_h
#define ds_znsesensitive_detector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

namespace ds_znse
{
class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String name);
    virtual ~SensitiveDetector();

    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4double fSignalAmplitude;
    G4double fSignAmp1;
    G4AnalysisManager* analManager;

    G4int nEvent;

protected:

};
}

#endif
