#include "globals.hh"

#include "TActionInitialization.hpp"
#include "TPrimaryGeneratorAction.hpp"
#include "TRunAction.hpp"
#include "TEventAction.hpp"


TActionInitialization::TActionInitialization()
   : G4VUserActionInitialization()
{}


TActionInitialization::~TActionInitialization()
{}


void TActionInitialization::BuildForMaster() const
{
   SetUserAction(new TRunAction());
}


void TActionInitialization::Build() const
{
   SetUserAction(new TPrimaryGeneratorAction());
   SetUserAction(new TRunAction());
   SetUserAction(new TEventAction());
}

