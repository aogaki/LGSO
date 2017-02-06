#include <math.h>
#include <map>

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VProcess.hh"
#include "G4HadronicProcess.hh"


#include "TSD.hpp"
#include "THit.hpp"


TSD::TSD(const G4String &name,
         const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

TSD::~TSD()
{}

void TSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new THitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool TSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   THit *newHit = new THit();


   G4Track *track = step->GetTrack();
   G4int trackID = track->GetTrackID();
   newHit->SetTrackID(trackID);

   G4int parentID = track->GetParentID();
   newHit->SetParentID(parentID);

   G4int stepNumber = track->GetCurrentStepNumber();
   newHit->SetStepNumber(stepNumber);

   G4ParticleDefinition *particle = track->GetDefinition();
   G4String particleName = particle->GetParticleName();
   newHit->SetParticleName(particleName);

   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   const G4VProcess *process = postStepPoint->GetProcessDefinedStep();
   if(process) newHit->SetProcessName(process->GetProcessName());
   
   G4ThreeVector position = track->GetPosition();
   newHit->SetPosition(position);
   
   G4ThreeVector vertexPosition = track->GetVertexPosition();
   newHit->SetVertexPosition(vertexPosition);
   
   G4ThreeVector momentum = postStepPoint->GetMomentum();
   newHit->SetMomentum(momentum);
   
   G4double kineticEnergy = postStepPoint->GetKineticEnergy();
   newHit->SetKineticEnergy(kineticEnergy);

   G4double depositedEnergy = step->GetTotalEnergyDeposit();
   newHit->SetDepositedEnergy(depositedEnergy);

   G4double time = postStepPoint->GetGlobalTime();
   newHit->SetTime(time);

   G4int isLast = step->IsLastStepInVolume();
   newHit->SetIsLastStep(isLast);

   
   fHitsCollection->insert(newHit);
   
   return true;
}
