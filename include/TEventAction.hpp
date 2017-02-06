#ifndef TEventAction_h
#define TEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "THit.hpp"


class TEventAction : public G4UserEventAction
{
public:
   TEventAction();
   virtual ~TEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   THitsCollection *GetHitsCollection(G4int hcID,
                                      const G4Event* event) const;
   
   G4int fHitsCollectionID;
};

#endif
