#ifndef PRIMARY_GENERATOR_MESSENGER_HH
#define PRIMARY_GENERATOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"  // 添加布尔命令
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
    G4UIcmdWithABool* fTestModeCmd;  // 测试模式命令
};

#endif
