#include "generator.hh"

Generator::Generator()
{
	fParticleGun = new G4ParticleGun(1); //one particle per event
}

Generator::~Generator()
{
	delete fParticleGun;
}

void Generator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="muon";
	G4ParticleDefinition *particle = particleTable->FindParticle("mu-");
	
	G4ThreeVector pos(0.,0.,2.5*m);
	G4ThreeVector mom(0.,0.,-1.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(1.00*GeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
