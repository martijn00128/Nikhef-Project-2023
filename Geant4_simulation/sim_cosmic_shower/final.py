import numpy as np
#import matplotlib.pyplot as plt
import sympy as sym
from numpy.random import rand as nrr
#import seaborn as sns
import os

day = 60*60*24
b = 10**-1/10**(-2.8*11)/1e9

def flux(E):return  b*E**(-2.8)

x = sym.symbols('x')
bins = np.linspace(11,14,101)
bins = 10**bins
number_flux = []

props = [0,0.118028982321487,0.222127179434798,0.313938821195182,0.394914071453941,0.466331933380236,0.529320451301746,0.584874528152206,0.633871639952047,0.677085695532315,0.715199260415958,0.748814337932615,0.778461877854436,0.804610162741792,0.827672204461043,0.848012267702351,0.865951623536359,0.881773623887154,0.895728177072724,0.908035695104042,0.918890575090299,0.928464269739068,0.936907995449891,0.944355120775618,0.950923272977197,0.956716195944947,0.961825388832135,0.966331551283082,0.970305858083132,0.973811083363593,0.976902592118436,0.979629214693751,0.982034018062516,0.984154986066957,0.986025619372901,0.987675464612425,0.989130581072572,0.990413952301501,0.991545849133373,0.992544149865943,0.993424622648080,0.994201174537498,0.994886071162577,0.995490130457822,0.996022893533024,0.996492775374977,0.996907197762102,0.997272706491353,0.997595074769007,0.997879394398391,0.998130156204862,0.998351320968339,0.998546381983778,0.998718420237730,0.998870153072485,0.999003977106466,0.999122006088788,0.999226104285901,0.999317915927661,0.999398891177920,0.999470309039846,0.999533297557768,0.999588851634618,0.999637848746418,0.999681062801999,0.999719176366882,0.999752791444399,0.999782438984321,0.999808587269208,0.999831649310927,0.999851989374168,0.999869928730002,0.999885750730353,0.999899705283539,0.999912012801570,0.999922867681556,0.999932441376205,0.999940885101916,0.999948332227242,0.999954900379443,0.999960693302411,0.999965802495298,0.999970308657749,0.999974282964549,0.999977788189830,0.999980879698585,0.999983606321160,0.999986011124529,0.999988132092533,0.999990002725839,0.999991652571079,0.999993107687539,0.999994391058768,0.999995522955600,0.999996521256332,0.999997401729114,0.999998178281004,0.999998863177629,0.999999467236924,1]

props = np.array(props);

dataset = 100000
coordx = (nrr(dataset)*2-1)*5
coordy = (nrr(dataset)*2-1)*5
#print(props)
direction = nrr(dataset)*2*np.pi
azi = nrr(dataset)*np.pi/180-np.pi/2
energys = []
for i in range(dataset):
    print("-----------------------------------------------%d---------------------------------------"%i)
    x = "%.2f*km"%coordx[i]
    y = "%.2f*km"%coordy[i]
    z = "84.*km"

    px = "%.2f"%(np.cos(direction[i])*np.cos(azi[i]))
    py = "%.2f"%(np.sin(direction[i])*np.cos(azi[i]))
    pz = "%.2f"%np.sin(azi[i])

    rand_num = nrr()
    loc = len(props[props<rand_num])
    energy = ((bins[loc+1]-bins[loc])*nrr()+bins[loc])/1e9
    with open("detector.txt",'r') as txt:
        lines = txt.readlines()
        with open("detector.cc",'w') as detector:
            for line in lines:
                line = line.replace("whatname","output_%i_%.2fGeV.txt"%(i,energy))
                detector.write(line)
    with open("generator.txt",'r') as txt:
        lines = txt.readlines()
        with open("generator.cc",'w') as generator:
            for line in lines:
                line = line.replace("whatx",x)
                line = line.replace("whaty",y)
                line = line.replace("whatz",z)
                line = line.replace("whatpx",px)
                line = line.replace("whatpy",py)
                line = line.replace("whatpz",pz)
                line = line.replace("whatmom","%.2f*GeV"%energy)
                generator.write(line)
    os.system("source /cvmfs/clicdp.cern.ch/software/allpix-squared/latest/x86_64-centos7-clang12-opt/setup.sh\n"+
              "cd build\n"+
              "cmake ..\n"+
              "make \n"+
              "./sim\n")
              
    



