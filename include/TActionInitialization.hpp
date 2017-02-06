#ifndef TActionInitialization_h
#define TActionInitialization_h 1

#include "G4VUserActionInitialization.hh"


class TActionInitialization : public G4VUserActionInitialization
{
public:
   TActionInitialization();
   virtual ~TActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:

};

#endif
