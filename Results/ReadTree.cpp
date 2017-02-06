#include <fstream>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>


void ReadTree(TString fileName = "test.root")
{
   TFile *file = new TFile(fileName, "READ");
   TTree *tree = (TTree*)file->Get("LGSO");
   tree->Print();

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

   TVector3 position;
   tree->SetBranchStatus("PositionX", 1);
   tree->SetBranchStatus("PositionY", 1);
   tree->SetBranchStatus("PositionZ", 1);
   tree->SetBranchAddress("PositionX", &position[0]);
   tree->SetBranchAddress("PositionY", &position[1]);
   tree->SetBranchAddress("PositionZ", &position[2]);
 
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


   ofstream fout("data.dat");
   const Int_t maxEvent = tree->GetEntries();
   tree->GetEntry(0);
   Int_t currentID = eventID;
   Int_t firstEvent = eventID;
   fout << "{{";
   for(Int_t iEvent = 0; iEvent < maxEvent; iEvent++){
      tree->GetEntry(iEvent);
      if(currentID != eventID){
         currentID = eventID;
         fout << "},\n{";
      }
      else if(eventID != firstEvent){
         fout << ",";
      }
      
      fout << "{"
           << "{"
           << position.x() <<","
           << position.y() <<","
           << position.z() <<"},"
           << depositedEnergy << "}";
   }
   
   fout << "}}" << endl;
   
   fout.close();
   delete tree;
   file->Close();
}
