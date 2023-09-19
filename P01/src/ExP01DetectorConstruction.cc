//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file persistency/P01/src/ExP01DetectorConstruction.cc
/// \brief Implementation of the ExP01DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "ExP01DetectorConstruction.hh"
#include "ExP01DetectorMessenger.hh"
#include "ExP01ChamberParameterisation.hh"
#include "ExP01MagneticField.hh"
#include "ExP01TrackerSD.hh"
#include "RootIO.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExP01DetectorConstruction::ExP01DetectorConstruction()
:G4VUserDetectorConstruction(),
 fSolidWorld(0), fLogicWorld(0), fPhysiWorld(0),
 fSolidTarget(0), fLogicTarget(0), fPhysiTargetDownstream(0), fPhysiTargetUpstream(0),
 fSolidTracker(0), fLogicTracker(0), fPhysiTracker(0), 
 fSolidChamber(0), fLogicChamber(0), fPhysiChamber(0), 
 fTargetMater(0), fChamberMater(0), fPMagField(0), fDetectorMessenger(0),
 fWorldLength(0.), fTargetLength(0.), fTargetHeight(0.),fTrackerLength(0.),
 fNbOfChambersX(0), fNbOfChambersY(0), fChamberWidth(0.), fChamberSpacing(0.)
{
  fPMagField = new ExP01MagneticField();
  fDetectorMessenger = new ExP01DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExP01DetectorConstruction::~ExP01DetectorConstruction()
{
  delete fPMagField;
  delete fDetectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExP01DetectorConstruction::doGasHeIsobMix(double frcHe, std::string matName, double pressure, double temperature){

	G4double density;
	G4int nel;

	G4double densityHe = 0.000166 *CLHEP::g/CLHEP::cm3;
	G4double densityIsoB = 0.00249 *CLHEP::g/CLHEP::cm3;
	G4double fractionHe = frcHe*CLHEP::perCent;

	density = fractionHe*densityHe + (1.0-fractionHe)*densityIsoB;
	density *= pressure/(1.0*CLHEP::atmosphere);

	G4Material *GasMix = new G4Material( matName, density, nel=3, kStateGas, temperature, pressure);

	G4NistManager* nist = G4NistManager::Instance();
	G4Element* He = nist->FindOrBuildElement("He");
	G4Element* C = nist->FindOrBuildElement("C");
	G4Element* H = nist->FindOrBuildElement("H");

	G4double atomicWeight_He = 4.002602 *CLHEP::g/CLHEP::mole;
	G4double atomicWeight_C = 12.0107 *CLHEP::g/CLHEP::mole;
	G4double atomicWeight_H = 1.00794 *CLHEP::g/CLHEP::mole;
	G4double pwHe = fractionHe*atomicWeight_He;
	G4double pwC = (1.0-fractionHe) * 4.0*atomicWeight_C;
	G4double pwH = (1.0-fractionHe) * 10.0*atomicWeight_H;
	G4double atomicWeightMix = pwHe + pwC + pwH ;

	pwHe/=atomicWeightMix;
	pwH/=atomicWeightMix;
	GasMix->AddElement(He, pwHe );
	GasMix->AddElement(H , pwH );
	GasMix->AddElement(C , 1.0-pwHe-pwH );

	fChamberMater =GasMix;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExP01DetectorConstruction::SetChamberMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial=G4NistManager::Instance()->FindOrBuildMaterial(materialName);
  if (pttoMaterial)
     {fChamberMater = pttoMaterial;
      fLogicChamber->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The chambers are " << fChamberWidth/cm << " cm of "
             << materialName << G4endl;
     }             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ExP01DetectorConstruction::Construct()
{
//--------- Material definition ---------

  //Vacuum + SiPm scintillator
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* scintillatorMaterial = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); // Replace with your scintillator material
  // Define the elemental materials (Au and Mo)
  G4Element* elAu = nist->FindOrBuildElement("Au");
  G4Element* elMo = nist->FindOrBuildElement("Mo");

  // Define the proportions or atomic fractions of each element in the mixture
  // For example, 50% Au and 50% Mo
  G4double fractionAu = 0.5;  // 50% Au
  G4double fractionMo = 0.5;  // 50% Mo

  // Calculate the density of the mixture (you can adjust this based on your requirements)
  G4double density = 10.0 * g/cm3;  // Adjust the density as needed

  // Create the composite material by defining a mixture
  G4Material* compositeMaterial = new G4Material("AuMoMixture", density, 2);
  compositeMaterial->AddElement(elAu, fractionAu);
  compositeMaterial->AddElement(elMo, fractionMo);

  fNbOfChambersX = 4;
  fNbOfChambersY = 4;

  fChamberWidth =1.0*cm;
  fChamberSpacing = 10.0*cm;

  fTrackerLength = 10*cm; // Full length of Tracker
  fTrackerHeight = 10*cm;
  fTrackerWidth = 10*cm;
  
  fTargetLength  = 4.0 * cm;                        // Full length of Trigger Scintillator
  fTargetHeight  = 1.0 * cm;                        // Full height of Trigger Scintillator
  fTargetWidth = 6.0 * cm;                           // Full width of Trigger Scintillator
//_______________for he iC4H10 mixture__________________//
   doGasHeIsobMix(90,"gas_mix",1.0*CLHEP::atmosphere,300);
   G4Material* materialCh = fChamberMater;

  fTargetMater  = scintillatorMaterial;
  
  //_____________________________________________________________//
  G4cout<<" material for Drift Chamber "<<materialCh;
  
  fWorldLength= 1.5*(fTargetLength+fTrackerLength);
   
  G4double targetSizeX  = 0.5*fTargetLength;    // Half length of the Target
  G4double targetSizeY = 0.5*fTargetWidth;
  G4double targetSizeZ = 0.5*fTargetHeight;
  G4double trackerSize = 0.5*fTrackerLength;   // Half length of the Tracker
  G4double tubeXSize = 2.0 * cm; // Adjust the size of the tube in the x-direction
  G4double tubeYSize = 2.0 * cm; // Adjust the size of the tube in the y-direction
  G4double tubeZSize = 10.0 * cm; // Adjust the size of the tube in the z-direction
  G4int numTubes = 4; // Change this value to nxn as needed

  
      
//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 

  G4double HalfWorldLength = 0.5*fWorldLength;
  
  fSolidWorld= new G4Box("world",HalfWorldLength,HalfWorldLength,HalfWorldLength);
  fLogicWorld= new G4LogicalVolume( fSolidWorld, vacuum, "World", 0, 0, 0);

  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  fPhysiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 fLogicWorld,      // its logical volume
                                 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // copy number
                                 
  //------------------------------ 
  // Target
  //------------------------------
  
  G4ThreeVector positionTargetUpstream = G4ThreeVector(0,0,-(targetSizeX+trackerSize));
  G4ThreeVector positionTargetDownstream = G4ThreeVector(0,0,+(targetSizeX+trackerSize));
   
  fSolidTarget = new G4Box("target",targetSizeX,targetSizeY,targetSizeZ);
  fLogicTarget = new G4LogicalVolume(fSolidTarget,fTargetMater,"Target",0,0,0);

  fPhysiTargetUpstream = new G4PVPlacement(0,               // no rotation
                                  positionTargetUpstream,  // at (x,y,z)
                                  fLogicTarget,     // its logical volume
                                  "Target_1",        // its name
                                  fLogicWorld,      // its mother  volume
                                  false,           // no boolean operations
                                  0);              // copy number 
  fPhysiTargetDownstream = new G4PVPlacement(0,               // no rotation
                                  positionTargetDownstream,  // at (x,y,z)
                                  fLogicTarget,     // its logical volume
                                  "Target_2",        // its name
                                  fLogicWorld,      // its mother  volume
                                  false,           // no boolean operations
                                  0);              // copy number 
  G4cout << "Target is " << fTargetLength/cm << " cm of " 
         << fTargetMater->GetName() << G4endl;

  //------------------------------ 
  // Tracker
  //------------------------------
  
  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);
  
  fSolidTracker = new G4Box("tracker",trackerSize,trackerSize,trackerSize);
  fLogicTracker = new G4LogicalVolume(fSolidTracker , vacuum, "Tracker",0,0,0);
  fPhysiTracker = new G4PVPlacement(0,              // no rotation
                                  positionTracker, // at (x,y,z)
                                  fLogicTracker,    // its logical volume
                                  "Tracker",       // its name
                                  fLogicWorld,      // its mother  volume
                                  false,           // no boolean operations
                                  0);              // copy number 

  //------------------------------ 
  // Tracker segments
  //------------------------------
  // 
  // An example of Parameterised volumes
  // dummy values for G4Box -- modified by parameterised volume
  
  fSolidChamber = new G4Box("chamber", 10*cm, 10*cm, 10*cm);
  fLogicChamber = new G4LogicalVolume(fSolidChamber, materialCh,"Chamber",0,0,0);
   
  G4double firstPosition = -trackerSize + 0.5*fChamberWidth;

  G4double firstLength = fTrackerLength/10;
  G4double lastLength  = fTrackerLength;
  
   
  G4VPVParameterisation* chamberParam = new ExP01ChamberParameterisation(  
                           fNbOfChambersX,          // NoChambers 
                           fNbOfChambersY,          // NoChambers 
                           firstPosition,         // Z of center of first 
                           fChamberSpacing,        // Z spacing of centers
 						               fChamberWidth,          // Width Chamber
 						               firstLength,           // lengthInitial
 						               lastLength);         // lengthFinal
                           
  G4cout<< " Chamber Width (cm) "<< fChamberWidth/cm <<G4endl;  // dummy value : kZAxis -- modified by parameterised volume

  fPhysiChamber = new G4PVParameterised(
                            "Chamber",       // their name
                            fLogicChamber,    // their logical volume
                            fLogicTracker,    // Mother logical volume
                            kUndefined,          // Are placed along this axis 
                            fNbOfChambersX*fNbOfChambersY,    // Number of chambers
                            chamberParam);   // The parametrisation
                    

// Create a parameterized volume for the cylinders inside the tubes
G4Tubs* tubeSolid = new G4Tubs("Tube", 0.*cm, 0.25*cm, 5*cm, 0.*deg, 360.*deg);
G4LogicalVolume* tubeLogical = new G4LogicalVolume(tubeSolid, compositeMaterial, "TubeLogical",0,0,0);
G4VPVParameterisation* TubesParam = new ExP01ChamberParameterisation(  
                           fNbOfChambersX,          // NoChambers 
                           fNbOfChambersY,          // NoChambers 
                           firstPosition,         // Z of center of first 
                           fChamberSpacing,        // Z spacing of centers
 						               fChamberWidth,          // Width Chamber
 						               firstLength,           // lengthInitial
 						               lastLength-10*cm);         // lengthFinal

G4PVParameterised* cylinderArray = new G4PVParameterised("CylindersPhysical", 
                            tubeLogical, 
                            fLogicChamber,
                            kUndefined,          // Are placed along this axis 
                            fNbOfChambersX*fNbOfChambersY,    // Number of chambers
                            TubesParam);   // The parametrisation

  G4cout << "There are " << fNbOfChambersX * fNbOfChambersY << " chambers in the tracker region. "
         << "The chambers are " << fChamberWidth/mm << " mm of " 
         << fChamberMater->GetName() << G4endl; 
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String trackerChamberSDname = "ExP01/TrackerChamberSD";
  ExP01TrackerSD* aTrackerSD = new ExP01TrackerSD( trackerChamberSDname );
  ExP01TrackerSD* aTargetSD = new ExP01TrackerSD( "ExP01/TargetSD" );
  SDman->AddNewDetector( aTrackerSD );
  SDman->AddNewDetector(aTargetSD);
  fLogicTarget->SetSensitiveDetector(aTargetSD);
  fLogicChamber->SetSensitiveDetector( aTrackerSD );

//--------- Visualization attributes -------------------------------

  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  fLogicWorld  ->SetVisAttributes(BoxVisAtt);  
  fLogicTarget ->SetVisAttributes(BoxVisAtt);
  fLogicTracker->SetVisAttributes(BoxVisAtt);
  
  G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(1, 0, 1,0.3));
  ChamberVisAtt->SetForceSolid(true);
  fLogicChamber->SetVisAttributes(ChamberVisAtt);
  
//--------- example of User Limits -------------------------------

  // below is an example of how to set tracking constraints in a given
  // logical volume(see also in N02PhysicsList how to setup the processes
  // G4StepLimiter or G4UserSpecialCuts).
    
  // Sets a max Step length in the tracker region, with G4StepLimiter
  //
  G4double maxStep = 1*cm;//proper "cut " value for secondary production
  G4UserLimits *lim= new G4UserLimits(maxStep);
  lim->SetUserMinEkine(0.0000001*eV);
  lim->SetUserMinRange(1*nm);
  fLogicChamber->SetUserLimits(lim);
  
  // Set additional contraints on the track, with G4UserSpecialCuts
  //
  G4double maxLength = 2*fTrackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  fLogicChamber->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,minEkin));
  
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExP01DetectorConstruction::SetTargetMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {fTargetMater = pttoMaterial;
      fLogicTarget->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
             << materialName << G4endl;
     }             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExP01DetectorConstruction::SetMagField(G4double fieldValue)
{
  fPMagField->SetFieldValue(fieldValue);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
