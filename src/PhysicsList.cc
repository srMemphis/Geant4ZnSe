#include "PhysicsList.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

PhysicsList::PhysicsList()
{   
    SetVerboseLevel(0);
    RegisterPhysics (new G4EmStandardPhysics_option4());
    RegisterPhysics (new G4OpticalPhysics());
    G4OpticalParameters *params = G4OpticalParameters::Instance();
    params->SetScintTrackSecondariesFirst(true);
    params->SetCerenkovTrackSecondariesFirst(true);
    // params->SetProcessActivation("Cerenkov", false);
}

PhysicsList::~PhysicsList()
{
}
