import os

add = os.getcwd();

energys = [0.05,0.1,0.25,0.5,1.0,2.0,5.0,10.0];

sim0 = open("sim0.txt","r").readlines();
detector0 = open("detector0.txt","r").readlines();
generator0 = open("generator0.txt","r").readlines();
for energy in energys:
	name = "\"output_%.2fGeV.txt\""%energy;
	with open("sim.cc","w") as sim:
		for line in sim0:
			line = line.replace("what",name);
			sim.write(line);
	with open("detector.cc","w") as detector:
		for line in detector0:
			line = line.replace("what",name);
			detector.write(line);
	with open("generator.cc","w") as generator:
		for line in generator0:
			line = line.replace("what","%.2f"%energy);
			generator.write(line);

	os.system(r"source /cvmfs/clicdp.cern.ch/software/allpix-squared/latest/x86_64-centos7-clang12-opt/setup.sh"+"\n"+
		  "cd build\n"+
		  "cmake ..\n"+
		  "make\n"+
		  "./sim\n")


