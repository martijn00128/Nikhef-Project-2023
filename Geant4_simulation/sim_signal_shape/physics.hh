#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

class physics_list : public G4VModularPhysicsList
{
public:
	physics_list();
	~physics_list();

};


#endif
