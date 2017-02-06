#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "THit.hpp"


G4ThreadLocal G4Allocator<THit> *THitAllocator = 0;


THit::THit()
   : G4VHit()
{}

THit::~THit()
{}

THit::THit(const THit & /*right*/)
   : G4VHit()
{}

const THit &
THit::operator=(const THit & /*right*/)
{
   return *this;
}

int THit::operator==(const THit & /*right*/) const
{
   return 0;
}
