#ifndef TSD_h
#define TSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include <map>

#include "THit.hpp"


class G4ParticleDefinition;
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class TSD: public G4VSensitiveDetector
{
public:
   TSD(const G4String &name,
       const G4String &hitsCollectionName);
   virtual ~TSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   THitsCollection *fHitsCollection;
   std::map<G4ParticleDefinition*,G4int> fParticleFlag;    
};

#endif
