#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include <iostream>

class PMT_Sen_Unit : public G4VSensitiveDetector
{
public:
	PMT_Sen_Unit(G4String);
	~PMT_Sen_Unit();

private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
