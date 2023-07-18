#include "physics.hh"

physics_list::physics_list()
{
	RegisterPhysics (new G4EmStandardPhysics);
	RegisterPhysics (new G4OpticalPhysics());
}

physics_list::~physics_list()
{}
