#include "DetectorConstruction.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

#include "SensitiveDetector.hh"

#include <iostream>

DetectorConstruction::DetectorConstruction():
    G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    ///////////////////////////////////////////////////////////////////////////
	// ---------------------------- Materials -------------------------------//
	///////////////////////////////////////////////////////////////////////////

	G4NistManager *nist = G4NistManager::Instance();
	G4double a, z, density;
	G4int nelements;

	// Air
	//
	G4Element* N = new G4Element("Nitrogen", "N", z = 7, a = 14.01 * g / mole);
	G4Element* O = new G4Element("Oxygen", "O", z = 8, a = 16.00 * g / mole);

	//G4Material* air = new G4Material("Air", density = 1.29 * mg / cm3, nelements = 2);
	G4Material* air = new G4Material("Air", density = 1e-19 * mg / cm3, nelements = 2);
	air->AddElement(N, 70. * perCent);
	air->AddElement(O, 30. * perCent);

	// Zinc Selenid
	//
	G4Element* Zn = new G4Element("Zinc", "Zn", z = 30, a = 65.38 * g / mole);
	G4Element* Se = new G4Element("Selen", "Se", z = 34, a = 78.96 * g / mole);

	G4Material* scintMat = new G4Material("ZnSe", density = 5.42 * g / cm3, nelements = 2);
	// scintMat->AddElement(nist->FindOrBuildElement("Zn"), 1);
	// scintMat->AddElement(nist->FindOrBuildElement("Se"), 1);
	scintMat->AddElement(Zn, 1);
	scintMat->AddElement(Se, 1);

	// Water
	G4Material* water = new G4Material("Water", density = 1.0 * g / cm3, nelements = 2);
	water->AddElement(nist->FindOrBuildElement("H"), 2);
	water->AddElement(nist->FindOrBuildElement("O"), 1);

	// ------------ Generate & Add Material Properties Table ------------

	ReadOpProps();

	std::vector<G4double> photonEnergy = ZnSe_ene;

	for (G4int i1 = 0; i1<photonEnergy.size(); i1++) {
		photonEnergy[i1] *= eV;
	}

	std::vector<G4double> refractiveIndex1 = ZnSe_rind;
	std::vector<G4double> refractiveIndex2;
	for (G4int i1 = 0; i1<photonEnergy.size(); i1++) {
		refractiveIndex2.push_back(1.0);
	}

	std::vector<G4double> absorption = ZnSe_abslen;
	for (G4int i1 = 0; i1<photonEnergy.size(); i1++) {
		absorption[i1] *= cm;
	}
	std::vector<G4double> scintilSlow = ZnSe_sc_fastcomp;


	G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

	myMPT1->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0], true, true);
	for(size_t i = 1; i < photonEnergy.size(); ++i)
  	{
    	myMPT1->AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
  	}

	myMPT1->AddProperty("ABSLENGTH", &photonEnergy[0], &absorption[0], true, true);
	for(size_t i = 1; i < photonEnergy.size(); ++i)
  	{
    	myMPT1->AddEntry("ABSLENGTH", photonEnergy[i], absorption[i]);
  	}
	
	myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilSlow, false, true);
	myMPT1->AddConstProperty("SCINTILLATIONYIELD", 0.4 /keV); //40. /keV
	myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
	myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns); //varies 20-70

	scintMat->SetMaterialPropertiesTable(myMPT1);

	// G4cout << "ZnSe(Te) G4MaterialPropertiesTable:" << G4endl;
	// myMPT1->DumpTable();

	// Air
	//

	G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
	myMPT2->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex2[0], true, true);
	for(size_t i = 1; i < photonEnergy.size(); ++i)
  	{
    	myMPT2->AddEntry("RINDEX", photonEnergy[i], refractiveIndex2[i]);
  	}

	// G4cout << "Air G4MaterialPropertiesTable:" << G4endl;
	// myMPT2->DumpTable();

	air->SetMaterialPropertiesTable(myMPT2);

	//-------------------------------------------------------------------------



	///////////////////////////////////////////////////////////////////////////
	// ----------------------------Geometry----------------------------------//
	///////////////////////////////////////////////////////////////////////////

	// Option to switch on/off checking of volumes overlaps
	G4bool checkOverlaps = true;


	// -------------------World------------------------------------------------

	G4double world_sizeXY = 10*cm;
	G4double world_sizeZ  = 10*cm;

	// shape
	G4Box* sWorld = new G4Box("World", 1.2*world_sizeXY/2, 1.2*world_sizeXY/2, 1.2*world_sizeZ/2);
	
	//volume
	G4LogicalVolume* logicWorld = new G4LogicalVolume(sWorld, air, "World");
	logicWorld->SetVisAttributes(G4VisAttributes(true, G4Colour(1., 1., 1., 0.1)));

	// phisical World
	G4VPhysicalVolume* physWorld = 
		new G4PVPlacement(0,                   //no rotation
						G4ThreeVector(),       //at (0,0,0)
						logicWorld,            //its logical volume
						"World",               //its name
						0,                     //its mother  volume
						false,                 //no boolean operation
						0,                     //copy number
						checkOverlaps);        //overlaps checking
						
	//--------------------Envelope---------------------------------------------
	G4Box *sEnv = new G4Box("Env", world_sizeXY/2, world_sizeXY/2, world_sizeZ/2);
	G4LogicalVolume *vEnv = new G4LogicalVolume(sEnv, air, "Env");
	vEnv->SetVisAttributes(G4VisAttributes(false));
	G4ThreeVector pos(0,0,0);
	new G4PVPlacement(0, pos, vEnv, "Env", logicWorld, 0, checkOverlaps);
	//-------------------------------------------------------------------------

	// G4RotationMatrix *rot = new G4RotationMatrix();
	// rot->rotateY(90*deg);

	//----------------------------scintillator---------------------------------
	float sc_thic = 4.00*mm;
	float shift = 0.01*mm;     //distance from origin to scint frontal plane

	G4Box *sScint = new G4Box("Scintillator", sc_thic/2, 6*mm/2, 6*mm/2);
	// G4Tubs *sScint = new G4Tubs("Scintillator", 0 , 3*mm, sc_thic/2, 0, CLHEP::twopi);

	G4LogicalVolume *vScint = new G4LogicalVolume(sScint, scintMat, "Scintillator");
	vScint->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0, 0.5, 0.1, 0.5)));
	pos = G4ThreeVector(shift + sc_thic/2, 0, 0);
	new G4PVPlacement(0, pos, vScint, "Scintillator", vEnv, 0, checkOverlaps);

	//-------------------------------------------------------------------------
	float gap = 0.01*mm;

	//----------------------------Detector-------------------------------------
	float det_thic = 2*mm;

	G4Box *sDetector = new G4Box("Detector", det_thic/2, 6*mm/2, 6*mm/2);
	// G4Tubs *sDetector = new G4Tubs("Scintillator", 0, 3*mm, det_thic/2, 0, CLHEP::twopi);

	G4LogicalVolume* vDetector = new G4LogicalVolume(sDetector, air, "Detector");
	vDetector->SetVisAttributes(G4VisAttributes(true, G4Colour(0.0, 0.0, 0.5, 0.5)));
	pos = G4ThreeVector(shift + sc_thic + gap + det_thic/2 , 0, 0);
	new G4PVPlacement(0, pos, vDetector, "Detector", vEnv, 0, checkOverlaps);

	// sensitive part
	fSensVol = vDetector;
	//-------------------------------------------------------------------------


	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
	G4VSensitiveDetector *SensDet = new SensitiveDetector("DetectorSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(SensDet);
	fSensVol->SetSensitiveDetector(SensDet);
}

void DetectorConstruction::ReadOpProps(){
	std::ifstream infile("../ZnSe_op_props.txt");
	if (!infile.is_open()) {
		G4cout << "[FATAL]: failed to open ZnSe_op_props.txt" << G4endl;
		return;
	}
	G4cout << "___________Reading prop file______________" << G4endl;
	G4double ene_tmp, fastcopm_tmp, rind_tmp, abslen_tmp;
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
	return;
}


