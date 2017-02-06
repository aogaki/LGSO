#ifndef TRunAction_h
#define TRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;

class TRunAction: public G4UserRunAction
{
public:
   TRunAction();
   virtual ~TRunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);

};

#endif
