#ifndef SENSITIVE_DETECTOR_HH
#define SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String name);
    virtual ~SensitiveDetector();

    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

protected:

    G4AnalysisManager* analysisManager;
    double fSignalAmplitude;
};

#endif
