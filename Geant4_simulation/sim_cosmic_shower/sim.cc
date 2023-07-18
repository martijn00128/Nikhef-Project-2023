#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4VModularPhysicsList.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"

int main(int argc, char** argv)
{
	G4RunManager *runManager = new G4RunManager();	

	runManager->SetUserInitialization(new NikhefProject());
	//runManager->SetUserInitialization(new physics_list());
	//-----------------------new physics list----------------------
	G4VModularPhysicsList* physics_list = new FTFP_BERT;
	physics_list->ReplacePhysics(new G4EmStandardPhysics_option4());
	G4OpticalPhysics *optical_physics = new G4OpticalPhysics();
	auto optical_params = G4OpticalParameters::Instance();
	optical_params->SetWLSTimeProfile("delta");
	optical_params->SetScintTrackSecondariesFirst(true);
	optical_params->SetCerenkovMaxPhotonsPerStep(100);
	optical_params->SetCerenkovMaxBetaChange(10.0);
	optical_params->SetCerenkovTrackSecondariesFirst(true);
	physics_list->RegisterPhysics(optical_physics);
	
	G4StepLimiterPhysics *stepLimitPhys = new G4StepLimiterPhysics();
	stepLimitPhys->SetApplyToAll(true);
	physics_list->RegisterPhysics(stepLimitPhys);
	physics_list->RegisterPhysics(new G4DecayPhysics());
	runManager->SetUserInitialization(physics_list);


	runManager->SetUserInitialization(new action_init());
	runManager->Initialize();
////
	G4UIExecutive *ui = new G4UIExecutive(argc, argv);
	
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();

	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	
	UImanager->ApplyCommand("/process/had/enableCRCoalescence true");
	//UImanager->ApplyCommand("/run/beamOn 1");
	
	UImanager->ApplyCommand("/vis/open OGL");
	UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 0 0");
	UImanager->ApplyCommand("/vis/drawVolume");
	UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
	UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
	
	
	ui->SessionStart();
	
	return 0;
}
