#ifndef PRIMARY_GENERATOR_MESSENGER_HH
#define PRIMARY_GENERATOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "globals.hh"

class PrimaryGeneratorAction;

class PrimaryGeneratorMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* action);
    virtual ~PrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand* command, G4String newValue);
    
private:
    PrimaryGeneratorAction* fAction;
    G4UIdirectory* fGunDir;
    G4UIcmdWithADouble* fCs137ActivityCmd;
    G4UIcmdWithABool* fTestModeCmd;
    G4UIcmdWithABool* fActivityModeCmd;  // 新增：活度模式命令
    G4UIcmdWithADouble* fSimulationTimeCmd;  // 新增：模拟时间命令
};

#endif
