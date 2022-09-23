#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include <string>

#include "SensitiveDetector.hh"

namespace ds_znse
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  
  // Define materials
  DefineMaterials();
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4cout << "DefineMaterials started" << G4endl;

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

    ///////////////////////////////////////////////////////////////////////////
	// ---------------------------- Materials -------------------------------//
	///////////////////////////////////////////////////////////////////////////

	G4double a, z, density;
	G4int nelements;

	// Air
	//
  	air = nist->FindOrBuildMaterial("G4_AIR");

	// Zinc Selenide
	//
	G4Element* Zn = new G4Element("Zinc", "Zn", z = 30, a = 65.38 * g / mole);
	G4Element* Se = new G4Element("Selen", "Se", z = 34, a = 78.96 * g / mole);

	scintMat = new G4Material("ZnSe", density = 5.2621 * g / cm3, nelements = 2);
	scintMat->AddElement(Zn, 1);
	scintMat->AddElement(Se, 1);

	//metal silicon (from g4ds)
	G4Element* Si = new G4Element("Silicium",  "Si", z=14, a=28.0855*g/mole );
	fMetalSilicon =  new G4Material("MetalSilicon",density = 2.33*g/cm3,1);
	fMetalSilicon->AddElement(Si,1.0);


	// ------------ Generate & Add Material Properties Table ------------
	// Zinc Selenide
	//
	ReadOpProps();

	std::vector<G4double> photonEnergy = ZnSe_ene;

	for (unsigned int i1 = 0; i1<photonEnergy.size(); i1++) {
		photonEnergy[i1] *= eV;
	}

	std::vector<G4double> refractiveIndex1 = ZnSe_rind;
	std::vector<G4double> refractiveIndex2;
	for (unsigned int i1 = 0; i1<photonEnergy.size(); i1++) {
		refractiveIndex2.push_back(1.0);
	}

	std::vector<G4double> abslen1 = ZnSe_abslen;
	for (unsigned int i1 = 0; i1<photonEnergy.size(); i1++) {
		ZnSe_abslen[i1] *= cm;
	}
	std::vector<G4double> scintilFast = ZnSe_sc_fastcomp;

	G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();


	myMPT1->AddProperty("RINDEX", photonEnergy, refractiveIndex1);
	myMPT1->AddProperty("ABSLENGTH", photonEnergy, abslen1);
	myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFast, false, true);
	myMPT1->AddConstProperty("SCINTILLATIONYIELD", 4. /keV); //40. /keV
	myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
	myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns); //varies 20-70

	scintMat->SetMaterialPropertiesTable(myMPT1);
	
	// Silicon

	G4MaterialPropertiesTable* Si_MPT = new G4MaterialPropertiesTable();
	Si_MPT->AddProperty("RINDEX", SiPM_ene, SiPM_rind);
	Si_MPT->AddProperty("ABSLENGTH", SiPM_ene, SiPM_abslen);

	fMetalSilicon->SetMaterialPropertiesTable(Si_MPT);

	// Air
	//
	
	G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
	myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2);
	air->SetMaterialPropertiesTable(myMPT2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

	bool checkOverlaps = true;

	// World
	//
	G4double world_sizeXY = 20 * cm;
	G4double world_sizeZ  = 30 * cm;
	G4Material* world_mat = air;

	G4Box* solidWorld =    
	new G4Box("World",                       //its name
		0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
		
	G4LogicalVolume* logicWorld =                         
	new G4LogicalVolume(solidWorld,          //its solid
						world_mat,           //its material
						"World");            //its name
									
	G4VPhysicalVolume* physWorld = 
	new G4PVPlacement(0,                     //no rotation
						G4ThreeVector(),       //at (0,0,0)
						logicWorld,            //its logical volume
						"World",               //its name
						0,                     //its mother  volume
						false,                 //no boolean operation
						0,                     //copy number
						checkOverlaps);        //overlaps checking
						
	// SCINT

	// float sc_width = 1*cm;
	G4double sc_thk = 1.8*mm;
	// G4double sc_rad = 6*mm;
	G4double det_thk = 2 * mm;
	float gap = 0.1*mm;
	auto rot = new G4RotationMatrix();
	rot->rotateY(0*deg);
	//----------------------------scintillator---------------------------------

	auto sScint = new G4Box("Scintillator", 6*mm/2, 6*mm/2, sc_thk/2);
	G4LogicalVolume *vScint = new G4LogicalVolume(sScint, scintMat, "Scintillator");  
	G4ThreeVector pos(0, 0, 0);
	
	new G4PVPlacement(rot, pos, vScint, "Scintillator", logicWorld, 0, checkOverlaps);
	//fScoringVolume = vScint;
	// Detector ------------------------------------
  
	auto sDetector = new G4Box("Detector", 6*mm/2, 6*mm/2, det_thk/2);
						//new G4Tubs("Detector", 0, sc_rad, det_thk, 0, 2*CLHEP::pi);
	vDetector = new G4LogicalVolume(sDetector, fMetalSilicon, "Detector");
	pos = G4ThreeVector(0, 0, sc_thk+gap+det_thk/2);
	new G4PVPlacement(rot, pos, vDetector, "Detector", logicWorld, 0, checkOverlaps);

	return physWorld;

}

void DetectorConstruction::ConstructSDandField()
{
	
  G4String name = "DetectorSD";
  G4VSensitiveDetector *SensDet = new SensitiveDetector(name);
  G4SDManager::GetSDMpointer()->AddNewDetector(SensDet);
  vDetector->SetSensitiveDetector(SensDet);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ReadOpProps(){
	std::ifstream infile("../ZnSe_op_props.txt");
	if (!infile.is_open()) {
		throw std::invalid_argument("[FATAL]: failed to open ZnSe_op_props.txt");
	}
	G4cout << "___________Read prop file for ZnSe______________" << G4endl;
	G4double ene_tmp, fastcopm_tmp, rind_tmp, rind1_temp, abslen_tmp, wlen_tmp;
	G4int len = 0;
	for (G4int i = 0; infile >> ene_tmp >> fastcopm_tmp >> rind_tmp >> abslen_tmp; i++){
		ZnSe_ene.push_back(ene_tmp);
		ZnSe_sc_fastcomp.push_back(fastcopm_tmp);
		ZnSe_rind.push_back(rind_tmp);
		ZnSe_abslen.push_back(abslen_tmp);
		len ++;
	}
	G4cout << "___________Read prop file, number of entries: " << len << "______________" << G4endl;
	infile.close();
	infile.clear();

	// metal silicon
	len = 0;
	infile.open("../metal_silicon_op_props.txt");
	if (!infile.is_open()) {
		throw std::invalid_argument("[FATAL]: failed to open SiPM op props file");
	}

	for (G4int i = 0; infile >> wlen_tmp >> abslen_tmp >> rind_tmp >> rind1_temp; i++){
		// SiPM_ene.push_back(h_Planck*c_light/(wlen_tmp*nm)*joule); // wlen to energy (h_Planck in G4 by default is joule*s!!!!!!!!!!!!!!!!!!!)
		SiPM_ene.push_back((1239.8 /(wlen_tmp))*eV);
		SiPM_rind.push_back(rind_tmp);
		SiPM_abslen.push_back(1/abslen_tmp*cm); // abs coef to abs len
		len ++;
		// G4cout << SiPM_ene[i] << " " << SiPM_rind[i] << " " << SiPM_abslen[i] << G4endl;
	}
	//just in case 
	std::reverse(SiPM_ene.begin(), SiPM_ene.end());
	std::reverse(SiPM_rind.begin(), SiPM_rind.end());
	std::reverse(SiPM_abslen.begin(), SiPM_abslen.end());

	G4cout << "___________Read SiPM prop file, number of entries: " << len << "______________" << G4endl;
	infile.close();
	return;
}

}

