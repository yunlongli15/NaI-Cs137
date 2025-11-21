#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList();
    
    virtual void SetCuts();
};

#endif
