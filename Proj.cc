#include <iostream>

//geant includes
#include "G4RunManagerFactory.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

// Proj includes
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInit.hh"

int main(int argc, char** argv)
{

    G4RunManager *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInit());
    runManager->Initialize();

    
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = 0;
    if ( argc == 1 ) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if ( ! ui ) { 
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else { 
        // interactive mode
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    delete visManager;
    delete runManager;

    return 0;
}
