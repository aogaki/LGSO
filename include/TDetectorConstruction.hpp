#ifndef TDetectorConstruction_h
#define TDetectorConstruction_h 1

#include <vector>

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"


using namespace std;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4GlobalMagFieldMessenger;
class G4VisAttributes;
class G4GenericMessenger;


class TDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   TDetectorConstruction();
   virtual ~TDetectorConstruction();

   G4VPhysicalVolume *Construct();
   void ConstructSDandField();

   void SetScintiL(G4double l);
   void SetScintiW(G4double w);
   void SetScintiT(G4double t);

private:
   void DefineMaterials();
   void DefineCommands();

   G4Material *fWorldMaterial;

   G4Material *fScintiMaterial;

   G4double fScintiL;
   G4double fScintiW;
   G4double fScintiT;

   G4VPhysicalVolume *fScintiPV;

   bool fCheckOverlap;

   vector<G4VisAttributes *> fVisAttributes;

   G4GenericMessenger *fMessenger;
};

#endif
