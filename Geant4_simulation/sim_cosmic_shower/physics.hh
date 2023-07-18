#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "FTFP_BERT.hh"

class physics_list : public G4VModularPhysicsList
{
public:
	physics_list();
	~physics_list();

};


#endif
