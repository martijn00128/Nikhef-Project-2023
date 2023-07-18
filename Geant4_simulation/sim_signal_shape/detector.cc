#include "detector.hh"

PMT_Sen_Unit::PMT_Sen_Unit(G4String name) : G4VSensitiveDetector(name)
{}

PMT_Sen_Unit::~PMT_Sen_Unit()
{}

G4bool PMT_Sen_Unit::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();


	G4int track_ID = track->GetTrackID();
	G4int parent_ID = track->GetParentID();
	G4int step_ID = track->GetCurrentStepNumber();
	G4double global_time = preStepPoint->GetLocalTime();
	G4double local_time = preStepPoint->GetGlobalTime();
	G4double proper_time = preStepPoint->GetProperTime();
	G4String particle_name = track->GetDefinition()->GetParticleName();
	G4ThreeVector xyz = preStepPoint->GetPosition();
	G4double energy = preStepPoint->GetTotalEnergy();
	
	G4VPhysicalVolume *Detector= preStepPoint->GetPhysicalVolume();
	G4String Detector_name = Detector->GetName();

	//G4cout << Detector_name << G4endl;
	
	std::fstream file("output_10.00GeV.txt", std::ios::app);
	file << Detector_name << "," << particle_name << "," << energy << "," << global_time << "," << local_time << "," << proper_time << "," \
	     << xyz[0] << "," << xyz[1] << "," << xyz[2] << "," << track_ID << "," <<parent_ID << "," << step_ID << std::endl;
	file.close();

	return true;
}

