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
	G4String particleName="cosmic";
	G4ParticleDefinition *particle = particleTable->FindParticle("proton");
	
	G4ThreeVector pos(0.*km,0.*km,84.*km);
	G4ThreeVector mom(0.00,0.00,-1.00);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(200*GeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
