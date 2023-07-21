# Nikhef-Project-2023

Authors:
- Martijn van Hamersveld
- Titus Veit
- Kelin Qiao

# Guide
This github contains all software that was used during our Nikhef project.

### NIKHEF_make_short_data.ipynb
Code used to reduce the size of the data files. The code takes in a full data run, finds data with coincidences, and writes this data to a new file.

### NIKHEF_amp_area_scatter.ipynb
Code that calculates the peak and the area of multiple data sets and plots these in a scatter plot.

### Data_analysis.ipynb
The main data analysis code, used for calibration of PMTs, finding pulse peaks, the area of pulses, and calculating the energy of a pulse.

### WaveDumpConfig_X742
The configuration file that was used for the Caen DAQ system, this file contains the thresholds, channels, and timings that were used in the data runs.

### Geant4_simulation
Folder containing all the files used for simulating the WCD, see the README in the folder for more information.