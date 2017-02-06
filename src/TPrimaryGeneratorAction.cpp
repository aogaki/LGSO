#include <math.h>

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "Randomize.hh"
#include "G4GenericMessenger.hh"
#include "G4ThreeVector.hh"

#include "TPrimaryGeneratorAction.hpp"


G4int nEveInPGA = 0;
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;

TPrimaryGeneratorAction::TPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(0)
{
  fSourceX = 0.;
  fSourceY = 0.;
  fSourceZ = -5.*cm;
  
   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable *parTable = G4ParticleTable::GetParticleTable();

   G4ParticleDefinition *particle;
   particle = parTable->FindParticle("gamma");
   fParticleGun->SetParticleDefinition(particle);

   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fSourceZ));
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
   fParticleGun->SetParticleEnergy(511.*keV);

   DefineCommands();

   fSourceW = 0.;
   fSourceL = 0.;
   fRealSourceFlag = true;
}

TPrimaryGeneratorAction::~TPrimaryGeneratorAction()
{
   delete fParticleGun;
   delete fMessenger;
}

void TPrimaryGeneratorAction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/LGSO/Source/", 
                                       "Gamma control");

   // x position
    G4GenericMessenger::Command &sourceXCmd
      = fMessenger->DeclareMethodWithUnit("X", "mm",
                                          &TPrimaryGeneratorAction::SetSourceX, 
                                          "Set the length of source.");
   sourceXCmd.SetParameterName("x", true);
   sourceXCmd.SetRange("x>=-1000. && x<1000.");
   sourceXCmd.SetDefaultValue("0");

   // y position
    G4GenericMessenger::Command &sourceYCmd
      = fMessenger->DeclareMethodWithUnit("Y", "mm",
                                          &TPrimaryGeneratorAction::SetSourceY, 
                                          "Set the length of source.");
   sourceYCmd.SetParameterName("y", true);
   sourceYCmd.SetRange("y>=-1000. && y<1000.");
   sourceYCmd.SetDefaultValue("0");

   // z position
    G4GenericMessenger::Command &sourceZCmd
      = fMessenger->DeclareMethodWithUnit("Z", "mm",
                                          &TPrimaryGeneratorAction::SetSourceZ, 
                                          "Set the length of source.");
   sourceZCmd.SetParameterName("z", true);
   sourceZCmd.SetRange("z>=-1000. && z<1000.");
   sourceZCmd.SetDefaultValue("0");

   // Length
    G4GenericMessenger::Command &sourceLCmd
      = fMessenger->DeclareMethodWithUnit("L", "mm",
                                          &TPrimaryGeneratorAction::SetSourceL, 
                                          "Set the length of source.");
   sourceLCmd.SetParameterName("l", true);
   sourceLCmd.SetRange("l>=0. && l<1000.");
   sourceLCmd.SetDefaultValue("0");

   // Width
    G4GenericMessenger::Command &sourceWCmd
      = fMessenger->DeclareMethodWithUnit("W", "mm",
                                          &TPrimaryGeneratorAction::SetSourceW, 
                                          "Set the width of source.");
   sourceWCmd.SetParameterName("w", true);
   sourceWCmd.SetRange("w>=0. && w<1000.");
   sourceWCmd.SetDefaultValue("0");

   // Set the flag
    G4GenericMessenger::Command &sourceFlagCmd
      = fMessenger->DeclareMethod("Flag", &TPrimaryGeneratorAction::SetRealSourceFlag, 
				  "Set the flag of source.");
   sourceFlagCmd.SetParameterName("flag", true);

}

void TPrimaryGeneratorAction::SetSourceX(G4double x)
{
   fSourceX = x;
}

void TPrimaryGeneratorAction::SetSourceY(G4double y)
{
   fSourceY = y;
}

void TPrimaryGeneratorAction::SetSourceZ(G4double z)
{
   fSourceZ = z;
}

void TPrimaryGeneratorAction::SetSourceL(G4double l)
{
   fSourceL = l;
}

void TPrimaryGeneratorAction::SetSourceW(G4double w)
{
   fSourceW = w;
}

void TPrimaryGeneratorAction::SetRealSourceFlag(G4String flag)
{
  if(flag == "true" || flag == "TRUE") // find the option ignoring case sensitive
    fRealSourceFlag = true;
  else fRealSourceFlag = false;
}

void TPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  G4ThreeVector pos = GetSourcePos();
  G4ThreeVector vec = GetSourceVec(pos);
  
  fParticleGun->SetParticlePosition(pos);
  fParticleGun->SetParticleMomentumDirection(vec);
  fParticleGun->GeneratePrimaryVertex(event);

   G4AutoLock lock(&mutexInPGA);
   nEveInPGA++;
   if (nEveInPGA % 1000 == 0)
      G4cout << nEveInPGA << " events done" << G4endl;
}

G4ThreeVector TPrimaryGeneratorAction::GetSourcePos()
{
  G4double x;
  G4double y;
  G4double z;
  
  if(fRealSourceFlag) {
    G4double theta = acos(1. - 2. * G4UniformRand());
    G4double phi = G4UniformRand() * 2. * CLHEP::pi;
    G4double r = 0.5 * pow(G4UniformRand(), 1./3.);
    z = r * cos(theta) +fSourceZ;
    x = r * sin(theta) * cos(phi) + fSourceX;
    y = r * sin(theta) * sin(phi) + fSourceY;
  } 
  else {
    x = fSourceX + fSourceW * (G4UniformRand() - 0.5);
    y = fSourceY + fSourceL * (G4UniformRand() - 0.5);
    z = fSourceZ;
  }
  
  return G4ThreeVector(x, y, z);
}

G4ThreeVector TPrimaryGeneratorAction::GetSourceVec(G4ThreeVector pos)
{
  G4double vx = 0.;
  G4double vy = 0.;
  G4double vz = 1.;
  
  if(fRealSourceFlag) {
     const G4double needleSize = 1.;
     const G4double halfSize = needleSize / 2.;
     G4double angleLimit = atan(needleSize / (2 * fSourceZ));

     G4double x = pos.x();
     G4double y = pos.y();
     G4double z = pos.z();
     
     while(1){
        G4double theta = acos(1. - G4UniformRand() * (1. - cos(angleLimit)));
        G4double phi = G4UniformRand() * 2. * CLHEP::pi;
        vz = cos(theta);
        vx = sin(theta) * cos(phi);
        vy = sin(theta) * sin(phi);

        G4double dz = (3 * fSourceZ + z) / vz;
        if(-halfSize < x + (vx * dz) && x + (vx * dz) < halfSize &&
           -halfSize < y + (vy * dz) && y + (vy * dz) < halfSize) break;
     }
  } 

  return G4ThreeVector(vx, vy, vz);
}
