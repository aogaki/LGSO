#ifndef TInteraction_h
#define TInteraction_h 1

#include <vector>

#include <TVector3.h>
#include <TString.h>


// Interaction point of primary Gamma
class TInteraction
{
public:
   TInteraction(){pos = TVector3(); e = 0.;};
   ~TInteraction(){};
   TInteraction(TVector3 position, Double_t ene, TString pro)
   {
      pos = position;
      e = ene;
      process = pro;
   };
   
   // Position
   TVector3 pos;

   // Deposited energy (Sum of daughters)
   Double_t e;

   // process name
   TString process;
   
private:
   
};

#ifdef __MAKECINT__
#pragma link C++ class TInteraction+;
#pragma link C++ class std::vector<TInteraction>+;
#endif

#endif
