#include "PrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

// constructor
PrimaryGeneratorAction::PrimaryGeneratorAction():
	G4VUserPrimaryGeneratorAction()
{
  	fParticleGun  = new G4ParticleGun(1);

	G4String particleName = "proton";
	G4double particleEnergy = 6.12*MeV;

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleEnergy(particleEnergy);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));         //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// particle pos
	G4ThreeVector pos(-1*cm ,0, 0);
  	fParticleGun->SetParticlePosition(pos);

}

// destructor
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//this function is called at the begining of ecah event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// particle momentum dir
	// G4double fi = G4UniformRand()*360*degree;
	// G4double theta = G4UniformRand()*10*degree;

  	// G4double ux = cos(theta);
  	// G4double uy = sin(theta)*sin(fi);
  	// G4double uz = sin(theta)*cos(fi);

  	// fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

	// aaah, yis. A Goat!
  	fParticleGun->GeneratePrimaryVertex(anEvent);
}
