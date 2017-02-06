#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

#include "TRunAction.hpp"


TRunAction::TRunAction()
   : G4UserRunAction()
{}

TRunAction::~TRunAction()
{}

void TRunAction::BeginOfRunAction(const G4Run */*run*/)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->SetVerboseLevel(1);
   G4cout << "Using " << anaMan->GetType()
          << " analysis manager" << G4endl;
   G4String fileName = "result";
   anaMan->OpenFile(fileName);

   anaMan->CreateNtuple("LGSO", "Simulation");
   anaMan->CreateNtupleIColumn(0, "EventID");
   anaMan->CreateNtupleIColumn(0, "TrackID");
   anaMan->CreateNtupleIColumn(0, "StepNumber");
   anaMan->CreateNtupleSColumn(0, "ParticleName");
   anaMan->CreateNtupleDColumn(0, "PositionX");
   anaMan->CreateNtupleDColumn(0, "PositionY");
   anaMan->CreateNtupleDColumn(0, "PositionZ");
   anaMan->CreateNtupleDColumn(0, "MomentumX");
   anaMan->CreateNtupleDColumn(0, "MomentumY");
   anaMan->CreateNtupleDColumn(0, "MomentumZ");
   anaMan->CreateNtupleDColumn(0, "DepositedEnergy");
   anaMan->CreateNtupleDColumn(0, "KineticEnergy");
   anaMan->CreateNtupleDColumn(0, "Time");
   anaMan->CreateNtupleIColumn(0, "IsLastStep");   
   anaMan->CreateNtupleSColumn(0, "ProcessName");   
   anaMan->CreateNtupleIColumn(0, "ParentID");   
   anaMan->CreateNtupleDColumn(0, "VertexPositionX");
   anaMan->CreateNtupleDColumn(0, "VertexPositionY");
   anaMan->CreateNtupleDColumn(0, "VertexPositionZ");
}

void TRunAction::EndOfRunAction(const G4Run */*run*/)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->Write();
   anaMan->CloseFile();
   delete G4AnalysisManager::Instance();
}
