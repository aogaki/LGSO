#ifndef TPrimaryGeneratorAction_h
#define TPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"


class G4Event;
class G4ParticleGun;
class G4GenericMessenger;

class TPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   TPrimaryGeneratorAction();
   virtual ~TPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

  void SetSourceX(G4double);
  void SetSourceY(G4double);
  void SetSourceZ(G4double);

  void SetSourceW(G4double);
  void SetSourceL(G4double);
  void SetRealSourceFlag(G4String);
  
private:
   G4ParticleGun *fParticleGun;

  G4GenericMessenger *fMessenger;
   void DefineCommands();

  G4double fSourceX;
  G4double fSourceY;
  G4double fSourceZ;
  
  G4double fSourceW;
  G4double fSourceL;
  G4bool fRealSourceFlag;

  G4ThreeVector GetSourcePos();
  G4ThreeVector GetSourceVec(G4ThreeVector);
};

#endif
