<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![MIT][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#Geant4DriftTubesSetup">Geant4 Drift Tubes Setup</a>
      <ul>
        <li><a href="#authors">Authors</a></li>
        <li><a href="#setup">Setup</a></li>
         <li> <a href="#instructions">Instructions</a></li>
      </ul>
    </li>
  </ol>
</details>

# Geant4DriftTubesSetup
This repository has been created starting from [ExP01](https://ecce-eic.github.io/doxygen/dir_bed8dccf306a29006b96259e27ac780d.html) provided by Geant4 and adapted to the geometry used in [INFN IDEA Drift Chamber Test Beams](https://arxiv.org/abs/2211.04220) (see Figures below).

The following `Initialization Classes` (usage of `G4RunManager::SetUserInitialization()` to set user mandatory classes to `RunManager`) are present:

- G4VUserDetectorConstruction: in this class I extensively used `G4Box`,`G4Tubes` classes to define respectively World Volume, Tracker Volume (Chamber + Inner Tubes) and Target Volumes (the two scintillators before and after the Tracker apparatus). Since the Chamber apparatus can be composed of parallelepiped material at different positions, I have defined the class `ChamberParameterisation`. The `ExP01TrackerSD` class makes the Chamber a sensitive detector from which I can retrieve information about hits.
- G4VUserPhysicsList: in this case I used the `FTFP_BERT` class,  recommended by Geant4 developers for HEP applications, it includes the standard EM physics  (“FTF” stands for FRITIOF string model (> 4 GeV) - “BERT” Bertini Cascade model (< 5 GeV), and “P” G4Precompound model used for de-excitation)
- G4VUserActionInitialization 

The following `Action Classes` (instantiated in `G4VUserActionInitialization` via G4RunManager::SetUserAction() and invoked during the event loop) have been used: 

- G4VUserPrimaryGeneratorAction: I searched for muons via the class `G4ParticleTable`, set a uniform angular distribution for the momentum direction and position, fixing the particle energy to 165 GeV
- G4UserRunAction: I used the `ExP01EventAction::BeginOfRunAction` method to create a root file in which I stored 1D and 2D histograms with information about step length, time, Energy deposited in each step by muon particles, and kinetic energy associated to each track. 
- G4UserEventAction: I used the `ExP01EventAction::EndOfEventAction` to print the number of trajectories stored in each event
- G4UserSteppingAction 

<img width="964" alt="Example of Geant4 Simulation, muon of 165 GeV and 10k events" src="https://github.com/bdanzi/Geant4DriftTubesSetup/blob/main/Screenshot%202023-09-19%20alle%2018.00.14.png">

<img width="964" alt="Example of Geant4 Simulation, muon of 165 GeV and 10k events" src="https://github.com/bdanzi/Geant4DriftTubesSetup/blob/main/Screenshot%202023-09-19%20alle%2018.00.40.png">

## Authors

- [Brunella D'Anzi](https://github.com/bdanzi) (University and INFN Bari)


## Setup

On Bari ReCAS and in the `testbeam_analysis\` directory of this repository:

```bash
$ source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh
$ source setDCDataReaderEnv.sh
$ bash compile.sh
$ bash submit_first.sh
```
On lxplus and in the `testbeam_analysis\` directory of this repository:

```bash
$ source setDCDataReaderEnv.sh
$ bash compile.sh
$ bash submit_first.sh
```
where the last line of code runs in parallel the `./read_data $path_to_your_run_files $i 0 -1 $sampling $N_1 $N_2 $N_3 $N_4 $binTimeInterval $dim $cut_scale` command in interactive mode or on HTcondor:

- i is the run number

- 0 -10 are the number of events to be processed (-1 to process the whole data set)

- $sampling is the sampling rate to be used in analysis

- $N_1 $N_2 $N_3 $N_4 are the cuts respectively on the signal amplitude, first and second derivatives before starting the search for electron peaks (see FindPeak-algo.C for further details)

- $binTimeInterval is the time interval (ns) starting from the beginning of the waveform in which the baseline and the rms is computed

- $dim are the waveform bins available from the data acquisition window

- $cut_scale is the threshold for electron peaks to be in the same primary ionization cluster

## Instructions

These macros find voltage amplitude peaks without any filter algorithm is applied to the waveform.
For each sample and each channel it is able to count how many events with an actual signal we have.
Config files and executables are created to run on more than one ROOT file (not available here, too much large in size).

```bash
$ submit_second.sh
```

It will produce in `executables\`:
- executable files `submit_executable_conversion*.sh` per each `run_*.root` file that has to be converted in a root file containing histos 
with the most important physical variables
- config files that can be run as job in recas for accelerating the process of the histos ROOT file generation

Moreover, it will produce:
- by using the plots_oldTestBeam.txt, per each .root file in the first column, per each channel in the second column,
per each event in the third column some physical quantities which are related to 
1) Number of Electron peaks, First Peak Time of Arrival and Last Peak Time of arrival distributions
2) Maxima
3) Charge Integral of the wavefunction (in pC)
4) Wave functions w & w/o peak arrows
5) Number of events per each channel which passes a voltage amplitude requirement for the waveform maximum of 5 mV
6) Number of Cluster distributions
7) Time Difference between two consecutive clusters
8) Time Difference between two consecutive Electrons
9) Cluster population (Number of Electron Peaks per Primary Ionization Cluster)

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/bdanzi/drifttubes_offline_analysis.svg?style=for-the-badge
[contributors-url]: https://github.com/bdanzi/drifttubes_offline_analysis/contributors

[forks-shield]: https://img.shields.io/github/forks/bdanzi/Geant4DriftTubesSetup.svg?style=for-the-badge
[forks-url]: https://github.com/bdanzi/drifttubes_offline_analysis/network/members

[stars-shield]: https://img.shields.io/github/stars/bdanzi/Geant4DriftTubesSetup.svg?style=for-the-badge
[stars-url]: https://github.com/bdanzi/drifttubes_offline_analysis/stargazers

[issues-shield]: https://img.shields.io/github/issues/bdanziGeant4DriftTubesSetup.svg?style=for-the-badge
[issues-url]: https://github.com/bdanzi/drifttubes_offline_analysis/issues

[license-shield]: https://img.shields.io/github/license/bdanzi/Geant4DriftTubesSetup.svg?style=for-the-badge
[license-url]: https://github.com/bdanzi/drifttubes_offline_analysis/blob/main/LICENSE.txt

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/brunella-d-anzi

