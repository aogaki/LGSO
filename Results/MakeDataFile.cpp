#include <iostream>
#include <fstream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>

#include "TInteraction.hpp"


void MakeDataFile(Double_t depth = 1.)
{
   TFile *file = TFile::Open("output.root", "READ");
   TTree *tree = NULL;
   file->GetObject("Gamma", tree);

   vector<TInteraction> *gamma = NULL;
   tree->SetBranchAddress("GammaData", &gamma);

   ofstream fout("data.dat");
   fout << "{\n";
   
   const Long64_t kNoEnt = tree->GetEntries();
   cout << kNoEnt << endl;
   for(Long64_t iEve = 0, firstFlag = 1; iEve < kNoEnt; iEve++){
      tree->GetEntry(iEve);
      if((*gamma)[0].pos.z() < depth){
         if(iEve < kNoEnt - 1 && firstFlag == 0) fout << "}, " << endl;
         fout << "{";
         for(UInt_t i = 0; i < (*gamma).size(); i++){
            if((*gamma)[i].pos.z() < depth && (*gamma)[i].pos.z() >= 0.){
               if(i != 0) fout <<", ";
               fout << "{{"
                    << (*gamma)[i].pos.x() <<", "
                    << (*gamma)[i].pos.y() <<", "
                    << (*gamma)[i].pos.z() <<"}, "
                    << (*gamma)[i].e <<"}";
               firstFlag = 0;
            }
            else break;
         }
      }
   }
   
   fout <<"}\n}"<< endl;
   fout.close();
}
