#ifndef ds_znseDetectorConstruction_h
#define ds_znseDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

namespace ds_znse
{

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume* Construct() override;
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    void ReadOpProps();
    virtual void ConstructSDandField();

  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    G4LogicalVolume*  fScoringVolume;
    std::vector<G4double> ZnSe_ene, ZnSe_rind, ZnSe_abslen, ZnSe_sc_fastcomp;
    std::vector<G4double> SiPM_ene, SiPM_rind, SiPM_abslen;

    G4Material* scintMat = nullptr;
    G4Material* air = nullptr;
    G4Material* fMetalSilicon = nullptr;

  protected:
    G4LogicalVolume* vDetector = nullptr;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

