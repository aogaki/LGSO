#include <fstream>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
#include <TH1.h>


class TTrack
{
public:
   TTrack():fTrackID(0), fParentID(0), fVertex(0., 0., 0.), fTotalDeposit(0.){};
   ~TTrack(){};
   
   Int_t fTrackID;
   Int_t fParentID;
   TVector3 fVertex;
   Double_t fTotalDeposit;
   Double_t fTime;
   
private:
   
};

TH1D *HisEne = new TH1D("HisEne", "Deposited Energy", 600, 0., 600.);


void ClearTrack(vector<TTrack> &t)
{
   for(UInt_t i = 0; i < t.size(); i++) t[i] = TTrack();
}

void PrintTrack(vector<TTrack> &t)
{

}

void test()
{
   TFile *file = new TFile("test.root", "READ");
   TTree *tree = (TTree*)file->Get("LGSO");

   tree->SetBranchStatus("*", 0);

   Int_t eventID;
   tree->SetBranchStatus("EventID", 1);
   tree->SetBranchAddress("EventID", &eventID);
   
   Int_t trackID;
   tree->SetBranchStatus("TrackID", 1);
   tree->SetBranchAddress("TrackID", &trackID);
   
   Int_t parentID;
   tree->SetBranchStatus("ParentID", 1);
   tree->SetBranchAddress("ParentID", &parentID);
   
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
 
   TVector3 vertex;
   tree->SetBranchStatus("VertexPositionX", 1);
   tree->SetBranchStatus("VertexPositionY", 1);
   tree->SetBranchStatus("VertexPositionZ", 1);
   tree->SetBranchAddress("VertexPositionX", &vertex[0]);
   tree->SetBranchAddress("VertexPositionY", &vertex[1]);
   tree->SetBranchAddress("VertexPositionZ", &vertex[2]);
 
   Double_t depositedEnergy;
   tree->SetBranchStatus("DepositedEnergy", 1);
   tree->SetBranchAddress("DepositedEnergy", &depositedEnergy);
   
   Double_t time;
   tree->SetBranchStatus("Time", 1);
   tree->SetBranchAddress("Time", &time);
   

   const Long64_t kNoEvent = tree->GetEntries();
   tree->GetEntry(0);
   Int_t currentID = eventID;
   
   vector<TTrack> track(200); // TrackID have to be smaller than 200
   ClearTrack(track);

   for(Long64_t i = 0; i < kNoEvent; i++){
      tree->GetEntry(i);
      if(trackID > 200) cout << "Think smart way!" << endl;
      
      if(eventID != currentID){
         ClearTrack(track);
         PrintTrack(track);
      }

      if(track[trackID].fTrackID != trackID){
         track[trackID].fTrackID = trackID;
         track[trackID].fParentID = parentID;
         track[trackID].fVertex = vertex;
         track[trackID].fTime = time;
      }
      track[trackID].fTotalDeposit += depositedEnergy;
   }
   
}
