#include <fstream>
#include <iostream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
#include <TH1.h>


TH1D *HisEne = new TH1D("HisEne", "Deposited Energy", 1000, 0., 0.);

void DepEne(TString fileName = "test.root")
{
   TChain *tree = new TChain("LGSO");
   tree->Add("../result_t*.root");

   //TFile *file = new TFile(fileName, "READ");
   //TTree *tree = (TTree*)file->Get("LGSO");
   //tree->Print();

   tree->SetBranchStatus("*", 0);

   Int_t eventID;
   tree->SetBranchStatus("EventID", 1);
   tree->SetBranchAddress("EventID", &eventID);
   
   //Int_t trackID;
   //tree->SetBranchStatus("TrackID", 1);
   //tree->SetBranchAddress("TrackID", &trackID);

   //Int_t stepNumber;
   //tree->SetBranchStatus("StepNumber", 1);
   //tree->SetBranchAddress("StepNumber", &stepNumber);
   
   Char_t particleName[256];
   tree->SetBranchStatus("ParticleName", 1);
   tree->SetBranchAddress("ParticleName", particleName);

   //TVector3 position;
   //tree->SetBranchStatus("PositionX", 1);
   //tree->SetBranchStatus("PositionY", 1);
   //tree->SetBranchStatus("PositionZ", 1);
   //tree->SetBranchAddress("PositionX", &position[0]);
   //tree->SetBranchAddress("PositionY", &position[1]);
   //tree->SetBranchAddress("PositionZ", &position[2]);
 
   //TVector3 momentum;
   //tree->SetBranchStatus("MomentumX", 1);
   //tree->SetBranchStatus("MomentumY", 1);
   //tree->SetBranchStatus("MomentumZ", 1);
   //tree->SetBranchAddress("MomentumX", &momentum[0]);
   //tree->SetBranchAddress("MomentumY", &momentum[1]);
   //tree->SetBranchAddress("MomentumZ", &momentum[2]);
 
   Double_t depositedEnergy;
   tree->SetBranchStatus("DepositedEnergy", 1);
   tree->SetBranchAddress("DepositedEnergy", &depositedEnergy);
   
   //Double_t kinematicEnergy;
   //tree->SetBranchStatus("KineticEnergy", 1);
   //tree->SetBranchAddress("KineticEnergy", &kinematicEnergy);

   //Double_t time;
   //tree->SetBranchStatus("Time", 1);
   //tree->SetBranchAddress("Time", &time);
   
   //Int_t isLastStep;
   //tree->SetBranchStatus("IsLastStep", 1);
   //tree->SetBranchAddress("IsLastStep", &isLastStep);


    const Int_t maxEvent = tree->GetEntries();
    cout << maxEvent << endl;
   tree->GetEntry(0);
   Int_t currentID = eventID;
   Double_t ene = 0.;
   for(Int_t iEvent = 0; iEvent < maxEvent; iEvent++){
     tree->GetEntry(iEvent);
     if(currentID != eventID){
       currentID = eventID;
       if(ene > 0.) HisEne->Fill(ene);
       ene = 0.;
     }
     else{
       ene += depositedEnergy;
     }
     
   }
   
   delete tree;
   //file->Close();
   HisEne->Draw();
}
