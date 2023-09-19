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
/// \file persistency/P01/include/ExP01TrackerHit.hh
/// \brief Definition of the ExP01TrackerHit class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExP01TrackerHit_h
#define ExP01TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Hit implementation for the persistency example

class ExP01TrackerHit : public G4VHit
{
  public:

      ExP01TrackerHit();
     ~ExP01TrackerHit();
      ExP01TrackerHit(const ExP01TrackerHit&);
      ExP01TrackerHit(G4int trackID,G4int chamberNb,G4double edep,G4double noIEdep,G4double gtime,G4double ptime,G4ThreeVector pos,G4ThreeVector end,G4ThreeVector mom,G4double len,G4String pcode);
      const ExP01TrackerHit& operator=(const ExP01TrackerHit&);
      G4bool operator==(const ExP01TrackerHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      virtual void Draw();
      virtual void Print();

  public:
  //_____________set methods__________//

      void SetTrackID  (G4int track)      { fTrackID = track; }
      void SetChamberNb(G4int chamb)      { fChamberNb = chamb; }
      void SetEdep     (G4double de)      { fEdep = de; }
      void SetDeltaE 	(G4double deltae) { fDeltaE=deltae;}
      void SetNoIEdep  (G4double nIde)    { fNoIEdep = nIde; }
      void SetGlobalTime(G4double gtime)  { fGlobalTime = gtime; }
      void SetProperTime(G4double ptime)  { fProperTime = ptime; }
      void SetStepLength(G4double len)    { fStepLength = len; }
      void SetMomentum  (G4ThreeVector mom) { fMomentum = mom; }
      void SetMomentumEnding (G4ThreeVector mom) { fMomentumEnding = mom; }
      void SetPos      (G4ThreeVector xyz) { fPos = xyz; }
      void SetPosEnding      (G4ThreeVector xyz) { fPosEnding = xyz; }
      void SetProcessCode(G4String process) {fProcessCode = process;}

      //_____________get methods__________//
      G4int GetTrackID() const     { return fTrackID; }
      G4int GetChamberNb() const   { return fChamberNb; }
      G4double GetEdep() const     { return fEdep; }
      G4double GetDeltaE() const   {return fDeltaE; }
      G4double GetNoIEdep() const  { return fNoIEdep; }
      G4double GetGlobalTime() const { return fGlobalTime; }
      G4double GetProperTime() const { return fProperTime; }
      G4double GetStepLength() const { return fStepLength; }
      G4ThreeVector GetPos() const { return fPos; }
      G4ThreeVector GetPosEnding() const { return fPosEnding; }
      G4ThreeVector GetMomentum() const { return fMomentum; }
      G4ThreeVector GetMomentumEnding() const { return fMomentumEnding; }
      G4String GetProcessCode() const {return fProcessCode;}

      
  private:
  
      G4int         fTrackID;
      G4int         fChamberNb;
      G4double      fEdep;
      G4double      fDeltaE;
      G4double      fNoIEdep;
      G4double      fGlobalTime;
      G4double      fProperTime;
      G4ThreeVector fPosEnding;
      G4ThreeVector fPos;
      G4ThreeVector fMomentum;
      G4ThreeVector fMomentumEnding;
      G4double      fStepLength;
      G4String      fProcessCode;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using ExP01TrackerHitsCollection = G4THitsCollection<ExP01TrackerHit>;

extern G4ThreadLocal G4Allocator<ExP01TrackerHit>* ExP01TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ExP01TrackerHit::operator new(size_t)
{
  if (!ExP01TrackerHitAllocator)
  {
    ExP01TrackerHitAllocator = new G4Allocator<ExP01TrackerHit>;
  }
  return (void *) ExP01TrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExP01TrackerHit::operator delete(void *aHit)
{
  ExP01TrackerHitAllocator->FreeSingle((ExP01TrackerHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
