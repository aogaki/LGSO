#include <time.h>
#include <fstream>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4HadronicProcessStore.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "FTFP_BERT_HP.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BIC_HP.hh"
#include "Shielding.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#include "G4TrajectoryParticleFilter.hh"
#endif

#include "TDetectorConstruction.hpp"
#include "TActionInitialization.hpp"

#include "Randomize.hh"


unsigned int GetRandomSeed()
{
   // Using /dev/urandom for generating random number.
   // If it is not, I have to think solution.
   unsigned int seed;
   std::ifstream file("/dev/urandom", std::ios::binary);
   if (file.is_open()) {
      char *memblock;
      int size = sizeof(int);
      memblock = new char[size];
      file.read(memblock, size);
      file.close();
      seed = *reinterpret_cast<unsigned int *>(memblock);
      delete[] memblock;
   } else {
      seed = 0;
   }

   return seed;
}

namespace {
void PrintUsage() {
   G4cerr << " Usage: " << G4endl;
   G4cerr << " SciFiTest [-m macro filename] [-a] [-s] [-h]" << G4endl
          << " -a Show all trajectory (default show only e- and gamma)\n"
          << " -h Show this usage" 
          << G4endl;
}
}

int main(int argc,char **argv)
{
   G4String macro = "";
   G4bool showAll = false;
   for ( G4int i = 1; i < argc; i++) {
      if      ( G4String(argv[i]) == "-m" ) macro = argv[++i];
      else if ( G4String(argv[i]) == "-a" ) showAll = true;
      else {
         PrintUsage();
         return 1;
      }
   }  

   // Choose the Random engine
   // Need both?
   unsigned int seed = GetRandomSeed();
   if (seed == 0) seed = time(0);
   G4cout << "\nseed = " << seed << G4endl;
   CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine(seed));
   G4Random::setTheEngine(new CLHEP::MTwistEngine(seed));
   
   // Construct the default run manager
#ifdef G4MULTITHREADED
   G4MTRunManager *runManager = new G4MTRunManager;
   runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
   //runManager->SetNumberOfThreads(1);
#else
   G4RunManager *runManager = new G4RunManager;
#endif
    
   // Mandatory user initialization classes
   runManager->SetUserInitialization(new TDetectorConstruction());
    
   //G4VModularPhysicsList *physicsList = new FTFP_BERT;
   //G4VModularPhysicsList *physicsList = new FTFP_BERT_HP;
   //G4VModularPhysicsList *physicsList = new QGSP_BERT;
   //G4VModularPhysicsList *physicsList = new QGSP_BIC_HP;
   G4VModularPhysicsList *physicsList = new Shielding;
   runManager->SetUserInitialization(physicsList);
    
   // User action initialization
   runManager->SetUserInitialization(new TActionInitialization());

   // Initialize Geant4 kernel
   runManager->Initialize();
  
#ifdef G4VIS_USE
   // Visualization manager construction
   G4VisManager *visManager = new G4VisExecutive;
   visManager->Initialize();
  
   if(!showAll){
      //Show only electron and gamma
      G4TrajectoryParticleFilter *filterp = new G4TrajectoryParticleFilter;
      filterp->Add("e-");
      filterp->Add("gamma");
      visManager->RegisterModel(filterp);
   }
#endif
  
   // Get the pointer to the User Interface manager
   G4UImanager *UImanager = G4UImanager::GetUIpointer();
    
   if (macro != "") {
      // execute an argument macro file if exist
      G4String command = "/control/execute ";
      G4String fileName = macro;
      UImanager->ApplyCommand(command+fileName);
   }
   else {
      // start interactive session
#ifdef G4UI_USE
      G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
      UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
      if (ui->IsGUI())
         UImanager->ApplyCommand("/control/execute icons.mac");
      ui->SessionStart();
      delete ui;
#endif
   }
    
   // Job termination
   // Free the store: user actions, physics_list and detector_description are
   // owned and deleted by the run manager, so they should not be deleted 
   // in the main() program !
  
#ifdef G4VIS_USE
   delete visManager;
#endif
   delete runManager;

   return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
