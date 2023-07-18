#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"

#include "detector.hh"

class NikhefProject : public G4VUserDetectorConstruction
{
public:
	NikhefProject();
	~NikhefProject();

	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logical_PMT214_Unit;
	G4LogicalVolume *logical_PMT335_Unit;
	G4LogicalVolume *logical_PMT350_Unit;
	G4LogicalVolume *logical_PMT374_Unit;
	virtual void ConstructSDandField();
};

#endif
