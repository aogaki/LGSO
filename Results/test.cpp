#include <fstream>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
#include <TH1.h>


const Int_t kTrackSize = 500;


TH1D *HisEne = new TH1D("HisEne", "Deposited Energy", 600, 0., 600.);


class TGamma
{
public:
   vector<TVector3> Interaction;
   vector<Double_t> TotalDeposit; 
};

class TTrack
{
public:
   TTrack():fTrackID(0), fParentID(0), fVertex(0., 0., 0.),
            fPosition(0., 0., 0.), fTotalDeposit(0.){};
   ~TTrack(){};
   
   Int_t fTrackID;
   Int_t fParentID;
   TVector3 fVertex;
   TVector3 fPosition;
   Double_t fTotalDeposit;
   
private:

};


Int_t FindDaughter(vector<TTrack> &t, Int_t id)
{
   if(id <= 1) return 1; // Error
   if(t[id].fParentID == 1) return id;
   else return FindDaughter(t, t[id].fParentID);
}

void ClearTrack(vector<TTrack> &t)
{
   for(UInt_t i = 0; i < t.size(); i++) t[i] = TTrack();
}

void PrintTrack(vector<TTrack> &t, ofstream &fout)
{
   // I beliebe in the vector is sorted by trackID

   // Summing deposited energy by daughter particles
   vector<TTrack> sorted;
/*
   for(auto &track: t){
      if(track.fTrackID > 0){
         if(track.fParentID <= 1) sorted.push_back(track);
         else sorted[FindDaughter(t, track.fTrackID) - 1].fTotalDeposit += track.fTotalDeposit;
      }
   }
*/
   for(Int_t i = 0; i < kTrackSize; i++){
      if(t[i].fTrackID > 0){
         if(t[i].fParentID <= 1) sorted.push_back(t[i]);
         else sorted[FindDaughter(t, t[i].fTrackID) - 1].fTotalDeposit += t[i].fTotalDeposit;
      }
   }

   
   
   // Summing deposited energy to interaction point
   Int_t kNoTrack = sorted.size();
   for(Int_t i = kNoTrack - 1; i >= 0; i--){
      TVector3 position1 = sorted[i].fVertex;
      if(i == 0) position1 = sorted[i].fPosition;

      for(Int_t j = i - 1; j >= 0; j--){
         TVector3 position2 = sorted[j].fVertex;
         if(j == 0) position2 = sorted[j].fPosition;
         
         if(position1 == position2){
            sorted[j].fTotalDeposit += sorted[i].fTotalDeposit;
            sorted.erase(sorted.begin() + i);
            break;
         }
      }
   }

   // remove 0
   kNoTrack = sorted.size(); // Use again? think smart way!
   for(Int_t i = 0; i < kNoTrack; i++){
      if(!(sorted[i].fTotalDeposit > 0.))
      sorted.erase(sorted.begin() + i);
   }

   kNoTrack = sorted.size(); // Use again? think smart way!
   fout << "{";
   for(Int_t i = 0; i < kNoTrack; i++){
      TVector3 position = sorted[i].fVertex;
      if(i == 0) position = sorted[i].fPosition;
      
      fout << "{";
      fout << "{"
           << position.x() << ","
           << position.y() << ","
           << position.z() << "},";
      fout << sorted[i].fTotalDeposit;
      if(i == kNoTrack - 1) fout << "}";
      else fout << "},";
   }
   fout << "}";

}

void test()
{
   ofstream fout("data.dat");
   fout << "{";

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
   
   Char_t processName[256];
   tree->SetBranchStatus("ProcessName", 1);
   tree->SetBranchAddress("ProcessName", processName);

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
   
   Long64_t start = 0; // For debug
   const Long64_t kNoEvent = tree->GetEntries();
   tree->GetEntry(start); 
   Int_t currentID = eventID;
   
   vector<TTrack> track(kTrackSize); // TrackID have to be smaller than 200
   ClearTrack(track);

   Double_t ene = 0.;
   
   for(Long64_t i = start; i < kNoEvent; i++){
      tree->GetEntry(i);
      if(trackID > kTrackSize) cout << "Think smart way!" << endl;
      //cout << i << endl;
      if(eventID != currentID){
         currentID = eventID;
         exit(0);
      }
      
      cout << trackID <<"\t"
           << parentID <<"\t"
           << depositedEnergy <<"\t"
           << position[0] <<"\t"
           << position[1] <<"\t"
           << position[2] <<"\t"
           << vertex[0] <<"\t"
           << vertex[1] <<"\t"
           << vertex[2] <<"\t"
           << time <<"\t"
           << endl;
      
   }

   fout << "}" << endl;
   fout.close();
}
