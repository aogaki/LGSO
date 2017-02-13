#include <fstream>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
#include <TH1.h>

#include "TInteraction.hpp"


const Int_t kTrackSize = 500;


TH1D *HisEne = new TH1D("HisEne", "Deposited Energy", 600, 0., 600.);


// Track class for NOT primary gamma
class TTrack
{
public:
   TTrack():TrackID(0), ParentID(0), Vertex(0., 0., 0.), TotalDeposit(0.){};
   ~TTrack(){};
   
   Int_t TrackID;
   Int_t ParentID;
   TVector3 Vertex;
   Double_t TotalDeposit;
   
private:

};


class TEvent
{
public:
   TEvent();
   ~TEvent();

   void SumEnergy();
   
   void Clear();
   void Print();
   
   vector<TInteraction> Gamma;
   vector<TTrack> Track;

   Int_t FindMother(Int_t id);

private:

   void ClearTrack();
   void ClearGamma();
   void SumDaughters();
   
};

TEvent::TEvent()
{
   Track.resize(kTrackSize);
}

TEvent::~TEvent()
{}

Int_t TEvent::FindMother(Int_t id)
{
   if(id <= 1) return 1; // Error
   if(Track[id].ParentID == 1) return id;
   else return FindMother(Track[id].ParentID);
}

void TEvent::Clear()
{
   ClearGamma();
   ClearTrack();
}

void TEvent::ClearGamma()
{
   Gamma.clear();
}

void TEvent::ClearTrack()
{
   for(UInt_t i = 0; i < Track.size(); i++) Track[i] = TTrack();
}

void TEvent::SumEnergy()
{
   SumDaughters();
   for(UInt_t i = Track.size() - 1; i > 0; i--){
      if(Track[i].TrackID == 0) continue;
      for(auto &gamma: Gamma){
         if(gamma.pos == Track[i].Vertex) gamma.e += Track[i].TotalDeposit;
      }
   }
}

void TEvent::SumDaughters()
{
   for(UInt_t i = Track.size() - 1; i > 0; i--){
      if(Track[i].TrackID == 0 || Track[i].ParentID == 1) continue;
      Int_t mother = FindMother(Track[i].TrackID);
      Track[mother].TotalDeposit += Track[i].TotalDeposit;
      //Track[i] = TTrack();
   }
}


void test()
{
   TEvent eve;

   //TFile *file = new TFile("test.root", "READ");
   //TTree *tree = (TTree*)file->Get("LGSO");
   TChain *tree = new TChain("LGSO");
   tree->Add("../result_t*");
   
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
 
   Double_t ene;
   tree->SetBranchStatus("DepositedEnergy", 1);
   tree->SetBranchAddress("DepositedEnergy", &ene);
   
   Double_t time;
   tree->SetBranchStatus("Time", 1);
   tree->SetBranchAddress("Time", &time);
   
   Long64_t start = 0; // For debug
   const Long64_t kNoEvent = tree->GetEntries();
   cout << kNoEvent << endl;
   tree->GetEntry(start); 
   Int_t currentID = eventID;
   
   //vector<TInteraction> gamma;
   vector<TInteraction> *pGamma = &eve.Gamma;
   TFile *output = new TFile("output.root", "RECREATE");
   TTree *outTree = new TTree("Gamma", "INteraction Gamma");
   outTree->Branch("GammaData", &pGamma);

   Double_t total = 0.;
   
   for(Long64_t i = start; i < kNoEvent; i++){
      tree->GetEntry(i);
      if(trackID > kTrackSize) cout << "Think smart way!" << endl;
      //cout << i << endl;
      if(eventID != currentID){
         currentID = eventID;
         eve.SumEnergy();
         outTree->Fill();
         eve.Clear();
      }

      if(trackID == 1) eve.Gamma.push_back(TInteraction(position, ene, TString(processName)));
      else{
         eve.Track[trackID].TrackID = trackID;
         eve.Track[trackID].ParentID = parentID;
         eve.Track[trackID].Vertex = vertex;
         eve.Track[trackID].TotalDeposit += ene;
      }

      total += ene;
   }

   cout << total << endl;
   
   // For last event
   eve.SumEnergy();
   outTree->Fill();
   eve.Clear();
   outTree->Write();
   output->Close();

   //file->Close();
}
