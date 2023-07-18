#include "action.hh"

action_init::action_init()
{}

action_init::~action_init()
{}

void action_init::Build() const
{
	Generator *generator = new Generator();
	SetUserAction(generator);
}
