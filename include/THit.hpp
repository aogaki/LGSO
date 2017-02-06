#ifndef THit_h
#define THit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class THit : public G4VHit
{
public:
   THit();
   virtual ~THit();
   THit(const THit &right);
   const THit &operator=(const THit &right);
   int operator==(const THit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

// Set and Get
   void SetTrackID(G4int trackID) {fTrackID = trackID;};
   G4int GetTrackID() {return fTrackID;};

   void SetParentID(G4int parentID) {fParentID = parentID;};
   G4int GetParentID() {return fParentID;};

   void SetStepNumber(G4int stepNumber) {fStepNumber = stepNumber;};
   G4int GetStepNumber() {return fStepNumber;};

   void SetParticleName(G4String particleName) {fParticleName = particleName;};
   G4String GetParticleName() {return fParticleName;};

   void SetProcessName(G4String particleName) {fProcessName = particleName;};
   G4String GetProcessName() {return fProcessName;};

   void SetPosition(G4ThreeVector position) {fPosition = position;};
   G4ThreeVector GetPosition() {return fPosition;};

   void SetVertexPosition(G4ThreeVector vertexPosition) {fVertexPosition = vertexPosition;};
   G4ThreeVector GetVertexPosition() {return fVertexPosition;};

   void SetMomentum(G4ThreeVector momentum) {fMomentum = momentum;};
   G4ThreeVector GetMomentum() {return fMomentum;};

   void SetDepositedEnergy(G4double depositedEnergy) {fDepositedEnergy = depositedEnergy;};
   G4double GetDepositedEnergy() {return fDepositedEnergy;};

   void SetKineticEnergy(G4double kineticEnergy) {fKineticEnergy = kineticEnergy;};
   G4double GetKineticEnergy() {return fKineticEnergy;};

   void SetTime(G4double time) {fTime = time;};
   G4double GetTime() {return fTime;};

   void SetIsLastStep(G4int isLastStep) {fIsLastStep = isLastStep;};
   G4int GetIsLastStep() {return fIsLastStep;};

private:
   G4int fTrackID;
   G4int fParentID;
   G4int fStepNumber;
   G4String fParticleName;
   G4String fProcessName;
   G4ThreeVector fPosition;
   G4ThreeVector fVertexPosition;
   G4ThreeVector fMomentum;
   G4double fDepositedEnergy;
   G4double fKineticEnergy;
   G4double fTime;
   G4int fIsLastStep;
   
};

typedef G4THitsCollection<THit> THitsCollection;

extern G4ThreadLocal G4Allocator<THit> *THitAllocator;

inline void *THit::operator new(size_t)
{
   if (!THitAllocator)
      THitAllocator = new G4Allocator<THit>;
   return (void *)THitAllocator->MallocSingle();
}

inline void THit::operator delete(void *hit)
{
   THitAllocator->FreeSingle((THit *) hit);
}

#endif
