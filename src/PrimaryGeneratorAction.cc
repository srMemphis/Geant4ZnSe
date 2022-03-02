#include "PrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

// constructor
PrimaryGeneratorAction::PrimaryGeneratorAction():
	G4VUserPrimaryGeneratorAction()
{
  	fParticleGun  = new G4ParticleGun(1);

	G4String particleName = "gamma";
	G4double particleEnergy = 1.*MeV;

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleEnergy(particleEnergy);

}

// destructor
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//this function is called at the begining of ecah event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{	

	// particle pos
	G4double alpha = G4UniformRand()*360*degree;
	G4double r = G4UniformRand()*0.5*mm;
	G4double x = 0;
	// G4double y = (G4UniformRand()*1 - 0.5) * mm;  // from -0.5mm to 0.5mm
	// G4double z = (G4UniformRand()*1 - 0.5) * mm;  // from -0.5mm to 0.5mm;
	G4double y = r*sin(alpha);
	G4double z = r*cos(alpha);

  	fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
	
	//particle momentum dir
	
	G4double fi = G4UniformRand()*360*degree;
	G4double theta = G4UniformRand()*89.9*degree;

  	G4double ux = cos(theta);
  	G4double uy = sin(theta)*sin(fi);
  	G4double uz = sin(theta)*cos(fi);

  	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

	// aaah, yis. A Goat!
  	fParticleGun->GeneratePrimaryVertex(anEvent);
}
