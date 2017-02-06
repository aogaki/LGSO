#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "g4root.hh"

#include "TEventAction.hpp"


TEventAction::TEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{}

TEventAction::~TEventAction()
{}

THitsCollection *TEventAction::GetHitsCollection(G4int hcID,
                                                 const G4Event *event)
const
{
   THitsCollection *hitsCollection 
      = static_cast<THitsCollection *>(
         event->GetHCofThisEvent()->GetHC(hcID));
  
   if ( ! hitsCollection ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID; 
      G4Exception("B4cEventAction::GetHitsCollection()",
                  "MyCode0003", FatalException, msg);
   }         

   return hitsCollection;
}

void TEventAction::BeginOfEventAction(const G4Event *)
{}

void TEventAction::EndOfEventAction(const G4Event *event)
{
   if(fHitsCollectionID == -1)
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScintiHC");

   THitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);

   G4int eventID = event->GetEventID();


   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   G4int nHit = hc->entries();
   for (G4int i = 0; i < nHit; i++) {
      THit *hit = (*hc)[i];

      anaMan->FillNtupleIColumn(0, 0, eventID);

      G4int trackID = hit->GetTrackID();
      anaMan->FillNtupleIColumn(0, 1, trackID);      

      G4int stepNumber = hit->GetStepNumber();
      anaMan->FillNtupleIColumn(0, 2, stepNumber);      

      G4String parName = hit->GetParticleName();
      anaMan->FillNtupleSColumn(0, 3, parName);      

      G4ThreeVector position = hit->GetPosition();
      anaMan->FillNtupleDColumn(0, 4, position.x());
      anaMan->FillNtupleDColumn(0, 5, position.y());
      anaMan->FillNtupleDColumn(0, 6, position.z());

      G4ThreeVector momentum = hit->GetMomentum();
      anaMan->FillNtupleDColumn(0, 7, momentum.x());
      anaMan->FillNtupleDColumn(0, 8, momentum.y());
      anaMan->FillNtupleDColumn(0, 9, momentum.z());

      G4double depEne = hit->GetDepositedEnergy();
      anaMan->FillNtupleDColumn(0, 10, depEne);
      
      G4double kinEne = hit->GetKineticEnergy();
      anaMan->FillNtupleDColumn(0, 11, kinEne);

      G4double time = hit->GetTime();
      anaMan->FillNtupleDColumn(0, 12, time);

      G4int isLast = hit->GetIsLastStep();
      anaMan->FillNtupleIColumn(0, 13, isLast);
      
      G4String processName = hit->GetProcessName();
      anaMan->FillNtupleSColumn(0, 14, processName);      

      G4int parentID = hit->GetParentID();
      anaMan->FillNtupleIColumn(0, 15, parentID);      

      G4ThreeVector vertexPosition = hit->GetVertexPosition();
      anaMan->FillNtupleDColumn(0, 16, vertexPosition.x());
      anaMan->FillNtupleDColumn(0, 17, vertexPosition.y());
      anaMan->FillNtupleDColumn(0, 18, vertexPosition.z());

      anaMan->AddNtupleRow();
   }

}
