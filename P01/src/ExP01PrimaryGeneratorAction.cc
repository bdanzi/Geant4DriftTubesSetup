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
/// \file persistency/P01/src/ExP01PrimaryGeneratorAction.cc
/// \brief Implementation of the ExP01PrimaryGeneratorAction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExP01PrimaryGeneratorAction.hh"
#include "ExP01DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExP01PrimaryGeneratorAction::ExP01PrimaryGeneratorAction(
                                               ExP01DetectorConstruction* myDC)
:G4VUserPrimaryGeneratorAction(),
 fParticleGun(0), fMyDetector(myDC)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // default particle

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);

  //solid angle
  //
  G4double alphaMin =  0*deg;      //alpha in [0,pi]
  G4double alphaMax = 30*deg;
  fCosAlphaMin = std::cos(alphaMin);
  fCosAlphaMax = std::cos(alphaMax);
  
  fPsiMin = 0*deg;       //psi in [0, 2*pi]
  fPsiMax = 30*deg;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExP01PrimaryGeneratorAction::~ExP01PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExP01PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  G4double positionZ = -0.5*(fMyDetector->GetWorldFullLength());
  G4double positionX = -0.5*(G4UniformRand() * 2.0 - 1.0)*(fMyDetector->GetWorldFullLength());
  G4double positionY = -0.5*(G4UniformRand() * 2.0 - 1.0)*(fMyDetector->GetWorldFullLength());
  G4double cosAlpha = fCosAlphaMin-G4UniformRand()*(fCosAlphaMin-fCosAlphaMax);
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi = fPsiMin + G4UniformRand()*(fPsiMax - fPsiMin);

  G4double ux = sinAlpha*std::cos(psi),
           uy = sinAlpha*std::sin(psi),
           uz = cosAlpha;

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
  fParticleGun->SetParticleEnergy(165.0*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(positionX,positionY,positionZ));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

