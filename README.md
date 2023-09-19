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
This repository has been created starting from [ExP01](https://ecce-eic.github.io/doxygen/dir_bed8dccf306a29006b96259e27ac780d.html) provided by Geant4 and adapted to the geometry used in [INFN IDEA Drift Chamber Test Beams](https://arxiv.org/abs/2211.04220) (see a test beam configuration example in the Figure below).

<img width="964" alt="Channel Schematics of July 2022 Beam Test" src="https://github.com/bdanzi/TestBeam2022/blob/main/Schermata%202022-09-10%20alle%2020.18.11.png">

The following `Initialization Classes` (usage of `G4RunManager::SetUserInitialization()` to set user mandatory classes to `RunManager`) are present:

- `G4VUserDetectorConstruction`: in this class I extensively used `G4Box`,`G4Tubes` classes to define the **World Volume** (vacuum), **Tracker Volume** (Chamber - Gas(He:90 IsoB:10, 90%) + Inner Tubes (Mo:50 Al:50) and **Target Volumes** (two Plastic Scintillators before and after the Tracker apparatus as triggers). User Limits for the tracker region have been set as well. Since the Chamber apparatus is composed of parallelepiped materials at different positions, I have defined the class `ChamberParameterisation`. The `ExP01TrackerSD` class makes the Chamber a sensitive detector from which I can retrieve information about its hits and kinematic processes.
- `G4VUserPhysicsList`: in this case, I used the`FTFP_BERT` class,  recommended by Geant4 developers for HEP applications, it includes the standard EM physics  (“FTF” stands for FRITIOF string model (> 4 GeV) - “BERT” Bertini Cascade model (< 5 GeV), and “P” G4Precompound model used for de-excitation)
- `G4VUserActionInitialization`

The following `Action Classes` (instantiated in `G4VUserActionInitialization` via G4RunManager::SetUserAction() and invoked during the event loop) have been used: 

- `G4VUserPrimaryGeneratorAction`: I searched for muons via the class `G4ParticleTable`, set a uniform angular distribution for the momentum direction and position, fixing the particle energy to 165 GeV
- `G4UserRunAction`: I used the `ExP01EventAction::BeginOfRunAction` method to create a root file in which I stored 1D and 2D histograms with information about step length, time, Energy deposited in each step by muon particles, and kinetic energy associated with each track. 
- `G4UserEventAction`: I used the `ExP01EventAction::EndOfEventAction` to print the number of trajectories stored in each event
- `G4UserSteppingAction` (+ `G4UserSteppingVerbose`): the method `void ExP01SteppingVerbose::StepInfo()` gives information at each step

Additional classes are:
- `RootIO`: It stores information about the sensitive detector in a ROOT file hists.root
- `ExP01MagneticField`: It can set a uniform value for the magnetic field, but in this case, an Electric Field would be needed to simulate EM showers in each parrallelepiped tube

<img width="964" alt="Example of Geant4 Simulation, muon of 165 GeV and 10k events" src="https://github.com/bdanzi/Geant4DriftTubesSetup/blob/main/Screenshot%202023-09-19%20alle%2018.00.14.png">

<img width="964" alt="Example of Geant4 Simulation, muon of 165 GeV and 10k events" src="https://github.com/bdanzi/Geant4DriftTubesSetup/blob/main/Screenshot%202023-09-19%20alle%2018.00.40.png">

## Authors

- [Brunella D'Anzi](https://github.com/bdanzi) (University and INFN Bari)


## Setup

Download of the source code [geant4-v11.1.2](https://geant4-dev.web.cern.ch/download/all) release on Mac OS X (M1 Ventura 13.3) and settings as follows for `cmake`:

```bash
$ mkdir geant4-v11.1.2-build
$ cd geant4-v11.1.2-build
$ cmake -DCMAKE_INSTALL_PREFIX=/Applications/geant4-v11.1.2-install /Users/brunelladanzi/Downloads/geant4-v11.1.2
$ cmake -DGEANT4_INSTALL_DATA=ON .
$ cmake -DGEANT4_USE_OPENGL_X11=ON .
$ cmake -DGEANT4_USE_QT=ON .
$ make -j8
$ make install
```


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

