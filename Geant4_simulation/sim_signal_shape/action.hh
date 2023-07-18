#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"

class action_init : public G4VUserActionInitialization
{
public:
	action_init();
	~action_init();

	virtual void Build() const;
};

#endif
