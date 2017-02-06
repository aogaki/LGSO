#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4GenericMessenger.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4SDManager.hh"

#include "TDetectorConstruction.hpp"
#include "TSD.hpp"


TDetectorConstruction::TDetectorConstruction()
   : G4VUserDetectorConstruction(),
     fWorldMaterial(0),
     fScintiMaterial(0),
     fScintiL(1.*mm),
     fScintiW(1.*mm),
     fScintiT(0.1*mm),
     fScintiPV(0),
     fCheckOverlap(true),
     fVisAttributes()
{
   DefineMaterials();
   DefineCommands();
}

TDetectorConstruction::~TDetectorConstruction()
{
   delete fMessenger;

   for (G4int i = 0; i < G4int(fVisAttributes.size()); ++i) {
      delete fVisAttributes[i];
   }

   delete fScintiMaterial;
}

void TDetectorConstruction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/LGSO/Geo/", 
                                       "Scinti control");

   // Thickness
    G4GenericMessenger::Command &scintiTCmd
      = fMessenger->DeclareMethodWithUnit("T", "mm",
                                          &TDetectorConstruction::SetScintiT, 
                                          "Set the thickness of scinti.");
   scintiTCmd.SetParameterName("t", true);
   scintiTCmd.SetRange("t>=0. && t<1000.");
   scintiTCmd.SetDefaultValue("0.5");

   // Length
    G4GenericMessenger::Command &scintiLCmd
      = fMessenger->DeclareMethodWithUnit("L", "mm",
                                          &TDetectorConstruction::SetScintiL, 
                                          "Set the length of scinti.");
   scintiLCmd.SetParameterName("l", true);
   scintiLCmd.SetRange("l>=0. && l<1000.");
   scintiLCmd.SetDefaultValue("100");

   // Width
    G4GenericMessenger::Command &scintiWCmd
      = fMessenger->DeclareMethodWithUnit("W", "mm",
                                          &TDetectorConstruction::SetScintiW, 
                                          "Set the width of primary scinti.");
   scintiWCmd.SetParameterName("w", true);
   scintiWCmd.SetRange("w>=0. && w<1000.");
   scintiWCmd.SetDefaultValue("100");
}

void TDetectorConstruction::SetScintiT(G4double t)
{
   fScintiT = t;
   G4Box *scinti = (G4Box*)(fScintiPV->GetLogicalVolume()->GetSolid());
   scinti->SetZHalfLength(fScintiT / 2.);

   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void TDetectorConstruction::SetScintiL(G4double l)
{
   fScintiL = l;
   G4Box *scinti = (G4Box*)(fScintiPV->GetLogicalVolume()->GetSolid());
   scinti->SetYHalfLength(fScintiL / 2.);

   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void TDetectorConstruction::SetScintiW(G4double w)
{
   fScintiW = w;
   G4Box *scinti = (G4Box*)(fScintiPV->GetLogicalVolume()->GetSolid());
   scinti->SetXHalfLength(fScintiW / 2.);

   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void TDetectorConstruction::DefineMaterials()
{
   G4NistManager *nistManager = G4NistManager::Instance();

   G4Material *air = nistManager->FindOrBuildMaterial("G4_AIR");

   G4Element *Si = nistManager->FindOrBuildElement("Si");
   G4Element *O = nistManager->FindOrBuildElement("O");
   G4Element *Lu = nistManager->FindOrBuildElement("Lu");
   G4Element *Gd = nistManager->FindOrBuildElement("Gd");

   G4Material *LGSO = new G4Material("LGSO", 6.5*g/cm3, 4, kStateSolid);
   LGSO->AddElement(Lu, 72.86*perCent);
   LGSO->AddElement(Gd, 3.45*perCent);
   LGSO->AddElement(Si, 6.16*perCent);
   LGSO->AddElement(O, 17.53*perCent);
  
   fWorldMaterial = air;
   fScintiMaterial = LGSO;
}

G4VPhysicalVolume *TDetectorConstruction::Construct()
{
   G4VSolid *worldS = new G4Box("World", 0.5 * m, 0.5 * m, 1.*m);
   G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fWorldMaterial, "World");
   G4VPhysicalVolume *worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World",
                                                  0, false, 0, fCheckOverlap);

   // Primary Scinti
   if(fScintiT <= 0.) fScintiT = 0.5*mm;
   G4VSolid *ScintiS = new G4Box("Scinti", fScintiW / 2., fScintiL / 2., fScintiT / 2.);
   //G4VSolid *ScintiS = new G4Box("Scinti", 50.*mm, 50.*mm, 0.5 * m);
   G4LogicalVolume *scintiLV = new G4LogicalVolume(ScintiS,
                                    fScintiMaterial,
                                    "Scinti");
   // Placed at center of the World.
   fScintiPV = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), scintiLV, "Scinti",
                                     worldLV, false, 0, fCheckOverlap);

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   worldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   scintiLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   G4Region *scintiRegion = new G4Region("ScintiRegion");
   scintiLV->SetRegion(scintiRegion);
   scintiRegion->AddRootLogicalVolume(scintiLV);
   
   G4ProductionCuts *cuts = new G4ProductionCuts();
   cuts->SetProductionCut(1.*nm, "gamma");
   cuts->SetProductionCut(1.*nm, "e-");
   scintiRegion->SetProductionCuts(cuts);
   
   return worldPV;
}

void TDetectorConstruction::ConstructSDandField()
{
   G4VSensitiveDetector *scintiSD = new TSD("ScintiSD", "ScintiHC");
   G4SDManager::GetSDMpointer()->AddNewDetector(scintiSD);
   SetSensitiveDetector("Scinti", scintiSD);
}
