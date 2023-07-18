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
#include "G4Material.hh"
#include "G4UserLimits.hh"

#include "detector.hh"

class NikhefProject : public G4VUserDetectorConstruction
{
public:
	NikhefProject();
	~NikhefProject();

	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logical_PMT214_Unit, *logical_PMT335_Unit, *logical_PMT350_Unit, *logical_PMT374_Unit;
	virtual void ConstructSDandField();

	G4Box *solidWorld, *solid_Atmosphere, *floor;
	G4LogicalVolume *logic_world, *logical_Atmosphere[40], *logical_floor[5];
	G4VPhysicalVolume *phys_world, *phys_Atmosphere[40], *phys_floor[5];

	G4Material *NK_Air[40], *worldMaterial, *NK_WATER, *NK_PMMA, *NK_GLASS, *NK_Si, *NK_Al, *NK_What;
};

#endif
