#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

// Forward declarations
class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    virtual void ConstructSDandField();

private:
    void ReadOpProps();
    std::vector<G4double> ZnSe_ene;
    std::vector<G4double> ZnSe_rind;
    std::vector<G4double> ZnSe_abslen;
    std::vector<G4double> ZnSe_sc_fastcomp;

    G4LogicalVolume* fSensVol;
};

#endif
