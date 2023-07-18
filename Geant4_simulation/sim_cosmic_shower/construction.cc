#include "construction.hh"


NikhefProject::NikhefProject()
{}

NikhefProject::~NikhefProject()
{}

G4VPhysicalVolume *NikhefProject::Construct()
{
//----------------------------------------Material--------------------------------------------------
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4double energyAIR[2] = {0.00001*eV,100*TeV};
	G4double rindexAIR[2] = {1.,1.};					//ignore air cherenkov
	G4MaterialPropertiesTable *mptAIR = new G4MaterialPropertiesTable();
	mptAIR->AddProperty("RINDEX",energyAIR,rindexAIR,2);			//make sure light can go out of boxes
	worldMaterial->SetMaterialPropertiesTable(mptAIR);

	G4Material *NK_WATER = nist->FindOrBuildMaterial("G4_WATER");
	//Water cherenkov properties
	G4double energyWATER[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
	G4double rindexWATER[2] = {1.33,1.33};					//equal refraction ratio
	G4MaterialPropertiesTable *mptWATER = new G4MaterialPropertiesTable();
	mptWATER->AddProperty("RINDEX",energyWATER,rindexWATER,2);
	NK_WATER->SetMaterialPropertiesTable(mptWATER);
	
	//PMMA Scintilation Properties
	G4Material *NK_PMMA = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	G4MaterialPropertiesTable *mptPMMA = new G4MaterialPropertiesTable();
	//PMMA Sci ability
	G4double energyPMMA[7] = {1.239841939*eV/0.70,1.239841939*eV/0.65,1.239841939*eV/0.6,\
				  1.239841939*eV/0.55,1.239841939*eV/0.50,1.239841939*eV/0.45,1.239841939*eV/0.4};
	G4double fraction[7] = {0.17244893,0.21343932,0.25697229,0.33055648,0.46170055,0.67791314,1.00670292};
	G4double rindexPMMA[7] = {1.49,1.49,1.49,1.49,1.49,1.49,1.49};
	mptPMMA->AddProperty("RINDEX",energyPMMA,rindexPMMA,7);
	mptPMMA->AddProperty("SCINTILLATIONCOMPONENT1",energyPMMA,fraction,7);
	mptPMMA->AddConstProperty("SCINTILLATIONYIELD",5./keV);
	mptPMMA->AddConstProperty("RESOLUTIONSCALE",1.0);
	mptPMMA->AddConstProperty("SCINTILLATIONTIMECONSTANT1",3.*ns);
	mptPMMA->AddConstProperty("SCINTILLATIONYIELD1",1.);
	NK_PMMA->SetMaterialPropertiesTable(mptPMMA);

	//Glass
	G4Material *NK_GLASS = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	G4double energyGLASS[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
	G4double rindexGLASS[2] = {1.5,1.5};					//make sure light goes out
	G4MaterialPropertiesTable *mptGLASS = new G4MaterialPropertiesTable();
	mptGLASS->AddProperty("RINDEX",energyGLASS,rindexGLASS,2);		//make sure light can go out of glass box
	NK_GLASS->SetMaterialPropertiesTable(mptGLASS);
	
	//Al Box and foil
	G4Material *NK_Al = nist->FindOrBuildMaterial("G4_Al");

	//Silicon plate
	G4Material *NK_Si = nist->FindOrBuildMaterial("G4_Si");
	G4double energySi[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
	//make sure Geant4 can detect photons. it is weird that you need this but it runs.
	G4double rindexSi[2] = {1.45,1.45};					
	G4MaterialPropertiesTable *mptSi = new G4MaterialPropertiesTable();
	mptSi->AddProperty("RINDEX",energySi,rindexSi,2);
	NK_Si->SetMaterialPropertiesTable(mptSi);

	//Atmosphere Components and Properties
	G4double density0 = 1.225*kg/m3;
	G4cout<< density0 <<"----------------------"<<std::endl;
	G4double p0 = 101325;
	G4double L = 0.0065;
	G4double g0 = 9.81;
	G4double M = 0.0289652;
	G4double R = 8.31446;
	G4double T0 = 293.15;
	G4double e = 2.718281828;

	G4double energyAir[2] = {0.00001*eV,100*TeV};
	G4double rindexAir[2] = {1.,1.};
	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX",energyAir,rindexAir,2);

	for(G4int i=0; i<40; i++){
		std::stringstream stri;
		G4double h = 80/40.*i;
		G4double density = density0*pow(e,-h/5.77);
		NK_Air[i] = new G4Material("NK_Air_"+stri.str(),density,1);
		NK_Air[i]->AddMaterial(nist->FindOrBuildMaterial("G4_AIR"),1);
		NK_Air[i]->SetMaterialPropertiesTable(mptAir);
	}
	
	//vacuum
	G4Material *NK_Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	//concrete
	G4Material *NK_Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

//----------------------------------------World-------------------------------------------------
	G4double worldX = 50*km;
	G4double worldY = 50*km;
	G4double worldZ = 85*km;

	//boundary of the world
	G4Box *solidWorld = new G4Box("solidWorld",worldX,worldY,worldZ); //Half length

	//for interaction
	G4LogicalVolume *logic_World = new G4LogicalVolume(solidWorld,NK_Vacuum,"logicWorld");

	//(rotation,place coordinate,logic world, name, mother volume,false,copy,overlapwarning)
	G4VPhysicalVolume *phys_World = new G4PVPlacement(0, G4ThreeVector(0. ,0. ,0.), logic_World, "physWorld",0,false,0,false);

//---------------------------------------Atmosphere---------------------------------------------

	solid_Atmosphere = new G4Box("solid_Atmosphere",worldX,worldY,1.*km);
	for (G4int i=0; i<40; i++){
		logical_Atmosphere[i] = new G4LogicalVolume(solid_Atmosphere,NK_Air[i],"logical_Atmosphere");
		phys_Atmosphere[i] = new G4PVPlacement(0,G4ThreeVector(0.,0.,2*i*km),logical_Atmosphere[i],"phys_Atmosphere",logic_World,false,i,true);
	}

//All detectors are located at y plus direction
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Nikhef Building~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	floor = new G4Box("solid_floor",25*m,25*m,0.05*m);
	for (G4int i=0;i<5; i++){
		logical_floor[i] = new G4LogicalVolume(floor,NK_Concrete,"logical_floor");
		phys_floor[i] = new G4PVPlacement(0,G4ThreeVector(0.,0.,3.*(i+1)*m),logical_floor[i],"phys_floor",logical_Atmosphere[0],false,i,true);	
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~WCD~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	//(name,x,y,z,thickness)
	//Metal_Box with 75cm*160cm*75cm
	G4Box *Metal_Box = new G4Box("Metal_Box",0.375*m,0.8*m,0.375*m);
	G4LogicalVolume *logical_Metal_Box = new G4LogicalVolume(Metal_Box,NK_Al,"logical_Metal_Box");
	G4VPhysicalVolume *phys_Metal_Box = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),logical_Metal_Box,"phys_Metal_Box",logical_Atmosphere[0],false,0,false);
	
	//inside Metal Box: 2cm thickness subtracted
	G4Box *inner_Metal_Box = new G4Box("inner_Metal_Box",0.365*m,0.79*m,0.365*m);
	G4LogicalVolume *logical_inner_Metal_Box = new G4LogicalVolume(inner_Metal_Box,worldMaterial,"logical_inner_Metal_Box");
	G4VPhysicalVolume *phys_inner_Metal_Box = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logical_inner_Metal_Box,"phys_inner_Metal_Box",logical_Metal_Box,false,0,false);

	//glass box 72cm*72cm*72*cm with thickness of 2cm
	G4Box *Glass_Box = new G4Box("Glass_Box",0.36*m,0.36*m,0.36*m);
	G4LogicalVolume *logical_Glass_Box = new G4LogicalVolume(Glass_Box,NK_GLASS,"logical_Glass_Box");
	G4VPhysicalVolume *phys_Glass_Box = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.005*m),logical_Glass_Box,"phys_Glass_Box",logical_inner_Metal_Box,false,0,false);

	//Water cube 70cm*70cm*70cm
	G4Box *Water_Cube = new G4Box("Water_Cube",0.35*m,0.35*m,0.35*m);
	G4LogicalVolume *logical_Water_Cube = new G4LogicalVolume(Water_Cube,NK_WATER,"logical_Water_Cube");
	//logical_Water_Cube->SetUserLimits(SStep);
	G4VPhysicalVolume *phys_Water_Cube = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logical_Water_Cube,"phys_Water_Cube",logical_Glass_Box,false,0,false);

	//Optical surface properties
	G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
	G4OpticalSurface *OpSurface = new G4OpticalSurface("WaterSurface");
	OpSurface->SetModel(glisur);
	OpSurface->SetType(dielectric_metal);					//photon will be absorded if is not reflected
	OpSurface->SetFinish(polished);						//smooth surface assumption
	G4double energyAl[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
	G4double reflectivity[2] = {0.75,0.75}; 				//reflectivity is 0.75, for cherenkov, including Scintillation
	G4MaterialPropertiesTable *mptAl = new G4MaterialPropertiesTable();
	mptAl->AddProperty("REFLECTIVITY",energyAl,reflectivity,2);
	OpSurface->SetMaterialPropertiesTable(mptAl);

	//Al foils
	G4Box *Al_foil1 = new G4Box("Al_foil1",0.36*m,0.00005*m,0.36*m);
	G4LogicalVolume *logical_Al_foil1 = new G4LogicalVolume(Al_foil1,NK_Al,"logical_Al_foil1");
	G4VPhysicalVolume *phys_Al_foil1 = new G4PVPlacement(0,G4ThreeVector(0.,-0.359995*m,0.),logical_Al_foil1,"phys_Al_foil1",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface1 = new G4LogicalBorderSurface("Surface1",phys_Glass_Box,phys_Al_foil1,OpSurface);

	G4Box *Al_foil2 = new G4Box("Al_foil2",0.36*m,0.36*m,0.000005*m);
	G4LogicalVolume *logical_Al_foil2 = new G4LogicalVolume(Al_foil2,NK_Al,"logical_Al_foil2");
	G4VPhysicalVolume *phys_Al_foil2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.359995*m),logical_Al_foil2,"phys_Al_foil2",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface2 = new G4LogicalBorderSurface("Surface2",phys_Glass_Box,phys_Al_foil2,OpSurface);

	G4Box *Al_foil3 = new G4Box("Al_foil3",0.36*m,0.36*m,0.000005*m);
	G4LogicalVolume *logical_Al_foil3 = new G4LogicalVolume(Al_foil3,NK_Al,"logical_Al_foil3");
	G4VPhysicalVolume *phys_Al_foil3 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.359995*m),logical_Al_foil3,"phys_Al_foil3",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface3 = new G4LogicalBorderSurface("Surface3",phys_Glass_Box,phys_Al_foil3,OpSurface);

	G4Box *Al_foil4 = new G4Box("Al_foil4",0.000005*m,0.36*m,0.36*m);
	G4LogicalVolume *logical_Al_foil4 = new G4LogicalVolume(Al_foil4,NK_Al,"logical_Al_foil4");
	G4VPhysicalVolume *phys_Al_foil4 = new G4PVPlacement(0,G4ThreeVector(0.359995*m,0.,0*m),logical_Al_foil4,"phys_Al_foil4",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface4 = new G4LogicalBorderSurface("Surface4",phys_Glass_Box,phys_Al_foil4,OpSurface);

	G4Box *Al_foil5 = new G4Box("Al_foil5",0.000005*m,0.36*m,0.36*m);
	G4LogicalVolume *logical_Al_foil5 = new G4LogicalVolume(Al_foil5,NK_Al,"logical_Al_foil5");
	G4VPhysicalVolume *phys_Al_foil5 = new G4PVPlacement(0,G4ThreeVector(-0.359995*m,0.,0*m),logical_Al_foil5,"phys_Al_foil5",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface5 = new G4LogicalBorderSurface("Surface5",phys_Glass_Box,phys_Al_foil5,OpSurface);

	G4Box *Al_foil6 = new G4Box("Al_foil6",0.36*m,0.00005*m,0.36*m);
	G4LogicalVolume *logical_Al_foil6 = new G4LogicalVolume(Al_foil6,NK_Al,"logical_Al_foil6");
	G4VPhysicalVolume *phys_Al_foil6 = new G4PVPlacement(0,G4ThreeVector(0.,0.359995*m,0.),logical_Al_foil6,"phys_Al_foil6",logical_Glass_Box,false,0,false);
	G4LogicalBorderSurface* Surface6 = new G4LogicalBorderSurface("Surface6",phys_Glass_Box,phys_Al_foil6,OpSurface);

	

//+++++++++++++++++++++++++++++++++++++++++Scintillator++++++++++++++++++++++++++++++++++++++++++++++++++++
	//--------------------Sci-1--------------------
	G4Box *Sci1 = new G4Box("Sci1",0.35*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci1 = new G4LogicalVolume(Sci1,NK_PMMA,"logical_Sci1");
	G4VPhysicalVolume *phys_Sci1 = new G4PVPlacement(0,G4ThreeVector(0,0,0.4*m),logical_Sci1,"phys_Sci1",logical_Atmosphere[0],false,0,true);

	G4Box *Sci1_Al_foil1 = new G4Box("Sci1_Al_foil1",0.35*m,0.00005*m,0.0175*m);
	G4LogicalVolume *logical_Sci1_Al_foil1 = new G4LogicalVolume(Sci1_Al_foil1,NK_Al,"logical_Sci1_Al_foil1");
	G4VPhysicalVolume *phys_Sci1_Al_foil1 = new G4PVPlacement(0,G4ThreeVector(0.,-0.349995*m,0.),logical_Sci1_Al_foil1,"phys_Sci1_Al_foil1",logical_Sci1,false,0,false);
	G4LogicalBorderSurface* Sci1_Surface1 = new G4LogicalBorderSurface("Sci1_Surface1",phys_Sci1,phys_Sci1_Al_foil1,OpSurface);

	G4Box *Sci1_Al_foil2 = new G4Box("Sci1_Al_foil2",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci1_Al_foil2 = new G4LogicalVolume(Sci1_Al_foil2,NK_Al,"logical_Sci1_Al_foil2");
	G4VPhysicalVolume *phys_Sci1_Al_foil2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.017495*m),logical_Sci1_Al_foil2,"phys_Sci1_Al_foil2",logical_Sci1,false,0,false);
	G4LogicalBorderSurface* Sci1_Surface2 = new G4LogicalBorderSurface("Sci1_Surface2",phys_Sci1,phys_Sci1_Al_foil2,OpSurface);

	G4Box *Sci1_Al_foil3 = new G4Box("Sci1_Al_foil3",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci1_Al_foil3 = new G4LogicalVolume(Sci1_Al_foil3,NK_Al,"logical_Sci1_Al_foil3");
	G4VPhysicalVolume *phys_Sci1_Al_foil3 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.017495*m),logical_Sci1_Al_foil3,"phys_Sci1_Al_foil3",logical_Sci1,false,0,false);
	G4LogicalBorderSurface* Sci1_Surface3 = new G4LogicalBorderSurface("Sci1_Surface3",phys_Sci1,phys_Sci1_Al_foil3,OpSurface);

	G4Box *Sci1_Al_foil4 = new G4Box("Sci1_Al_foil4",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci1_Al_foil4 = new G4LogicalVolume(Sci1_Al_foil4,NK_Al,"logical_Sci1_Al_foil4");
	G4VPhysicalVolume *phys_Sci1_Al_foil4 = new G4PVPlacement(0,G4ThreeVector(0.349995*m,0.,0*m),logical_Sci1_Al_foil4,"phys_Sci1_Al_foil4",logical_Sci1,false,0,false);
	G4LogicalBorderSurface* Sci1_Surface4 = new G4LogicalBorderSurface("Sci1_Surface4",phys_Sci1,phys_Sci1_Al_foil4,OpSurface);

	G4Box *Sci1_Al_foil5 = new G4Box("Sci1_Al_foil5",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci1_Al_foil5 = new G4LogicalVolume(Sci1_Al_foil5,NK_Al,"logical_Sci1_Al_foil5");
	G4VPhysicalVolume *phys_Sci1_Al_foil5 = new G4PVPlacement(0,G4ThreeVector(-0.349995*m,0.,0*m),logical_Sci1_Al_foil5,"phys_Sci1_Al_foil5",logical_Sci1,false,0,false);
	G4LogicalBorderSurface* Sci1_Surface5 = new G4LogicalBorderSurface("Sci1_Surface5",phys_Sci1,phys_Sci1_Al_foil5,OpSurface);

	G4Box *Sci1_Al_foil6 = new G4Box("Sci1_Al_foil6",0.35*m,0.00005*m,0.0175*m);
	G4LogicalVolume *logical_Sci1_Al_foil6 = new G4LogicalVolume(Sci1_Al_foil6,NK_Al,"logical_Sci1_Al_foil6");
	G4VPhysicalVolume *phys_Sci1_Al_foil6 = new G4PVPlacement(0,G4ThreeVector(0.,0.349995*m,0.),logical_Sci1_Al_foil6,"phys_Sci1_Al_foil6",logical_Sci1,false,0,false);

	//--------------------Sci-2--------------------
	G4Box *Sci2 = new G4Box("Sci2",0.35*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci2 = new G4LogicalVolume(Sci2,NK_PMMA,"logical_Sci2");
	G4VPhysicalVolume *phys_Sci2 = new G4PVPlacement(0,G4ThreeVector(0,0,-0.4*m),logical_Sci2,"phys_Sci2",logical_Atmosphere[0],false,0,true);

	G4Box *Sci2_Al_foil1 = new G4Box("Sci2_Al_foil1",0.35*m,0.00005*m,0.0175*m);
	G4LogicalVolume *logical_Sci2_Al_foil1 = new G4LogicalVolume(Sci2_Al_foil1,NK_Al,"logical_Sci2_Al_foil1");
	G4VPhysicalVolume *phys_Sci2_Al_foil1 = new G4PVPlacement(0,G4ThreeVector(0.,-0.349995*m,0.),logical_Sci2_Al_foil1,"phys_Sci2_Al_foil1",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface1 = new G4LogicalBorderSurface("Sci2_Surface1",phys_Sci2,phys_Sci2_Al_foil1,OpSurface);

	G4Box *Sci2_Al_foil2 = new G4Box("Sci2_Al_foil2",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci2_Al_foil2 = new G4LogicalVolume(Sci2_Al_foil2,NK_Al,"logical_Sci2_Al_foil2");
	G4VPhysicalVolume *phys_Sci2_Al_foil2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.017495*m),logical_Sci2_Al_foil2,"phys_Sci2_Al_foil2",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface2 = new G4LogicalBorderSurface("Sci2_Surface2",phys_Sci2,phys_Sci2_Al_foil2,OpSurface);

	G4Box *Sci2_Al_foil3 = new G4Box("Sci2_Al_foil3",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci2_Al_foil3 = new G4LogicalVolume(Sci2_Al_foil3,NK_Al,"logical_Sci2_Al_foil3");
	G4VPhysicalVolume *phys_Sci2_Al_foil3 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.017495*m),logical_Sci2_Al_foil3,"phys_Sci2_Al_foil3",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface3 = new G4LogicalBorderSurface("Sci2_Surface3",phys_Sci2,phys_Sci2_Al_foil3,OpSurface);

	G4Box *Sci2_Al_foil4 = new G4Box("Sci2_Al_foil4",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci2_Al_foil4 = new G4LogicalVolume(Sci2_Al_foil4,NK_Al,"logical_Sci2_Al_foil4");
	G4VPhysicalVolume *phys_Sci2_Al_foil4 = new G4PVPlacement(0,G4ThreeVector(0.349995*m,0.,0*m),logical_Sci2_Al_foil4,"phys_Sci2_Al_foil4",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface4 = new G4LogicalBorderSurface("Sci2_Surface4",phys_Sci2,phys_Sci2_Al_foil4,OpSurface);

	G4Box *Sci2_Al_foil5 = new G4Box("Sci2_Al_foil5",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci2_Al_foil5 = new G4LogicalVolume(Sci2_Al_foil5,NK_Al,"logical_Sci2_Al_foil5");
	G4VPhysicalVolume *phys_Sci2_Al_foil5 = new G4PVPlacement(0,G4ThreeVector(-0.349995*m,0.,0*m),logical_Sci2_Al_foil5,"phys_Sci2_Al_foil5",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface5 = new G4LogicalBorderSurface("Sci2_Surface5",phys_Sci2,phys_Sci2_Al_foil5,OpSurface);

	G4Box *Sci2_Al_foil6 = new G4Box("Sci2_Al_foil6",0.35*m,0.000005*m,0.0175*m);
	G4LogicalVolume *logical_Sci2_Al_foil6 = new G4LogicalVolume(Sci2_Al_foil6,NK_Al,"logical_Sci2_Al,foil6");
	G4VPhysicalVolume *phys_Sci2_Al_foil6 = new G4PVPlacement(0,G4ThreeVector(0.,0.349995*m,0.),logical_Sci2_Al_foil6,"phys_Sci2_Al_foil6",logical_Sci2,false,0,false);
	G4LogicalBorderSurface* Sci2_Surface6 = new G4LogicalBorderSurface("Sci2_Surface6",phys_Sci2,phys_Sci2_Al_foil6,OpSurface);

	//--------------------Sci-3--------------------
	G4Box *Sci3 = new G4Box("Sci3",0.35*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci3 = new G4LogicalVolume(Sci3,NK_PMMA,"logical_Sci3");
	G4VPhysicalVolume *phys_Sci3 = new G4PVPlacement(0,G4ThreeVector(10.*m,0,-0.4*m),logical_Sci3,"phys_Sci3",logical_Atmosphere[0],false,0,true);

	G4Box *Sci3_Al_foil1 = new G4Box("Sci3_Al_foil1",0.35*m,0.00005*m,0.0175*m);
	G4LogicalVolume *logical_Sci3_Al_foil1 = new G4LogicalVolume(Sci3_Al_foil1,NK_Al,"logical_Sci3_Al_foil1");
	G4VPhysicalVolume *phys_Sci3_Al_foil1 = new G4PVPlacement(0,G4ThreeVector(0.,-0.349995*m,0.),logical_Sci3_Al_foil1,"phys_Sci3_Al_foil1",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface1 = new G4LogicalBorderSurface("Sci3_Surface1",phys_Sci3,phys_Sci3_Al_foil1,OpSurface);

	G4Box *Sci3_Al_foil2 = new G4Box("Sci3_Al_foil2",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci3_Al_foil2 = new G4LogicalVolume(Sci3_Al_foil2,NK_Al,"logical_Sci3_Al_foil2");
	G4VPhysicalVolume *phys_Sci3_Al_foil2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.017495*m),logical_Sci3_Al_foil2,"phys_Sci3_Al_foil2",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface2 = new G4LogicalBorderSurface("Sci3_Surface2",phys_Sci3,phys_Sci3_Al_foil2,OpSurface);

	G4Box *Sci3_Al_foil3 = new G4Box("Sci3_Al_foil3",0.35*m,0.35*m,0.000005*m);
	G4LogicalVolume *logical_Sci3_Al_foil3 = new G4LogicalVolume(Sci3_Al_foil3,NK_Al,"logical_Sci3_Al_foil3");
	G4VPhysicalVolume *phys_Sci3_Al_foil3 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.017495*m),logical_Sci3_Al_foil3,"phys_Sci3_Al_foil3",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface3 = new G4LogicalBorderSurface("Sci3_Surface3",phys_Sci3,phys_Sci3_Al_foil3,OpSurface);

	G4Box *Sci3_Al_foil4 = new G4Box("Sci3_Al_foil4",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci3_Al_foil4 = new G4LogicalVolume(Sci3_Al_foil4,NK_Al,"logical_Sci3_Al_foil4");
	G4VPhysicalVolume *phys_Sci3_Al_foil4 = new G4PVPlacement(0,G4ThreeVector(0.349995*m,0.,0*m),logical_Sci3_Al_foil4,"phys_Sci3_Al_foil4",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface4 = new G4LogicalBorderSurface("Sci3_Surface4",phys_Sci3,phys_Sci3_Al_foil4,OpSurface);

	G4Box *Sci3_Al_foil5 = new G4Box("Sci3_Al_foil5",0.000005*m,0.35*m,0.0175*m);
	G4LogicalVolume *logical_Sci3_Al_foil5 = new G4LogicalVolume(Sci3_Al_foil5,NK_Al,"logical_Sci3_Al_foil5");
	G4VPhysicalVolume *phys_Sci3_Al_foil5 = new G4PVPlacement(0,G4ThreeVector(-0.349995*m,0.,0*m),logical_Sci3_Al_foil5,"phys_Sci3_Al_foil5",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface5 = new G4LogicalBorderSurface("Sci3_Surface5",phys_Sci3,phys_Sci3_Al_foil5,OpSurface);

	G4Box *Sci3_Al_foil6 = new G4Box("Sci3_Al_foil6",0.35*m,0.000005*m,0.0175*m);
	G4LogicalVolume *logical_Sci3_Al_foil6 = new G4LogicalVolume(Sci3_Al_foil6,NK_Al,"logical_Sci3_Al,foil6");
	G4VPhysicalVolume *phys_Sci3_Al_foil6 = new G4PVPlacement(0,G4ThreeVector(0.,0.349995*m,0.),logical_Sci3_Al_foil6,"phys_Sci3_Al_foil6",logical_Sci3,false,0,false);
	G4LogicalBorderSurface* Sci3_Surface6 = new G4LogicalBorderSurface("Sci3_Surface6",phys_Sci3,phys_Sci3_Al_foil6,OpSurface);



//===================================PMT Defining===========================================
	//PMT Unit Define
	//All PMT model are assumed to be squareds with length of their diameters
	//Materials can be anything since it's only photon sensitive
	//thickness can be ignored
	
	G4double PMT214_half_length = 2.14/2.*cm;
	G4double PMT335_half_length = 3.35/2.*cm;
	G4double PMT350_half_length = 3.50/2.*cm;
	G4double PMT374_half_length = 3.74/2.*cm;
	G4double PMT_grid = 1.;

	G4Tubs *PMT214_Unit = new G4Tubs("PMT214_Unit",0,PMT214_half_length,0.01*cm,0.,2*M_PI*rad);
	logical_PMT214_Unit = new G4LogicalVolume(PMT214_Unit,NK_Si,"logical_PMT214_Unit");
	
	G4Tubs *PMT335_Unit = new G4Tubs("PMT335_Unit",0,PMT335_half_length,0.01*cm,0.,2*M_PI*rad);
	logical_PMT335_Unit = new G4LogicalVolume(PMT335_Unit,NK_Si,"logical_PMT335_Unit");
	
	G4Tubs *PMT374_Unit = new G4Tubs("PMT374_Unit",0,PMT374_half_length,0.01*cm,0.,2*M_PI*rad);
	logical_PMT374_Unit = new G4LogicalVolume(PMT374_Unit,NK_Si,"logical_PMT374_Unit");

	G4Tubs *PMT350_Unit = new G4Tubs("PMT350_Unit",0,PMT350_half_length,0.01*cm,0.,2*M_PI*rad);
	logical_PMT350_Unit = new G4LogicalVolume(PMT350_Unit,NK_Si,"logical_PMT350_Unit");
	
	G4RotationMatrix* Rotation = new G4RotationMatrix();
	Rotation->rotateX(90*deg);
	Rotation->rotateY(0.);
	Rotation->rotateZ(0.);
	G4VPhysicalVolume *phys_PMT1 = new G4PVPlacement(Rotation,G4ThreeVector(-10.*cm,0.36*m-0.002*cm,-30.*cm),\
							logical_PMT335_Unit,"phys_PMT1",logical_Glass_Box,false,0,false);
	G4VPhysicalVolume *phys_PMT2 = new G4PVPlacement(Rotation,G4ThreeVector(20.*cm,0.36*m-0.002*cm,-30.*cm),\
							logical_PMT335_Unit,"phys_PMT2",logical_Glass_Box,false,0,false);
	G4VPhysicalVolume *phys_PMT3 = new G4PVPlacement(Rotation,G4ThreeVector(0.*cm,0.36*m-0.002*cm,-20.*cm),\
							logical_PMT374_Unit,"phys_PMT3",logical_Glass_Box,false,0,false);
	G4VPhysicalVolume *phys_PMT4 = new G4PVPlacement(Rotation,G4ThreeVector(10.*cm,0.36*m-0.002*cm,-9.*cm),\
							logical_PMT214_Unit,"phys_PMT4",logical_Glass_Box,false,0,false);	
	G4VPhysicalVolume *phys_PMT5 = new G4PVPlacement(Rotation,G4ThreeVector(-20.*cm,0.36*m-0.002*cm,-9.*cm),\
							logical_PMT214_Unit,"phys_PMT5",logical_Glass_Box,false,0,false);
	
	G4VPhysicalVolume *phys_Sci1_PMT = new G4PVPlacement(Rotation,G4ThreeVector(0.*cm,0.35*m-0.001*cm,0.*cm),\
							logical_PMT350_Unit,"phys_Sci1_PMT",logical_Sci1,false,0,false);
	G4VPhysicalVolume *phys_Sci2_PMT = new G4PVPlacement(Rotation,G4ThreeVector(0.*cm,0.35*m-0.001*cm,0.*cm),\
							logical_PMT350_Unit,"phys_Sci2_PMT",logical_Sci2,false,0,false);
	G4VPhysicalVolume *phys_Sci3_PMT = new G4PVPlacement(Rotation,G4ThreeVector(0.*cm,0.35*m-0.001*cm,0.*cm),\
							logical_PMT350_Unit,"phys_Sci3_PMT",logical_Sci3,false,0,false);


	return phys_World;
}

void NikhefProject::ConstructSDandField()
{
	PMT_Sen_Unit *sens_unit = new PMT_Sen_Unit("PMT_Sen_Unit");
//	PMT_Sen_Unit *sens_unit335 = new PMT_Sen_Unit("PMT_Sen_Unit_335");
	logical_PMT214_Unit->SetSensitiveDetector(sens_unit);
	logical_PMT335_Unit->SetSensitiveDetector(sens_unit);
	logical_PMT350_Unit->SetSensitiveDetector(sens_unit);
	logical_PMT374_Unit->SetSensitiveDetector(sens_unit);
}


