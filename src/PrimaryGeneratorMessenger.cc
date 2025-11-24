#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* action)
 : fAction(action)
{
    // 创建命令目录
    fGunDir = new G4UIdirectory("/gun/");
    fGunDir->SetGuidance("PrimaryGenerator control commands.");
    
    // 创建Cs-137活度命令
    fCs137ActivityCmd = new G4UIcmdWithADouble("/gun/cs137Activity", this);
    fCs137ActivityCmd->SetGuidance("Set Cs-137 activity concentration in Bq/m3");
    fCs137ActivityCmd->SetParameterName("activity", true);
    fCs137ActivityCmd->SetDefaultValue(1.0);
    fCs137ActivityCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    // 创建测试模式命令
    fTestModeCmd = new G4UIcmdWithABool("/gun/testMode", this);
    fTestModeCmd->SetGuidance("Enable test mode (fixed gamma source position)");
    fTestModeCmd->SetParameterName("testMode", true);
    fTestModeCmd->SetDefaultValue(false);
    fTestModeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    // 新增：活度模式命令
    fActivityModeCmd = new G4UIcmdWithABool("/gun/activityMode", this);
    fActivityModeCmd->SetGuidance("Enable activity mode (event count based on activity and time)");
    fActivityModeCmd->SetParameterName("activityMode", true);
    fActivityModeCmd->SetDefaultValue(false);
    fActivityModeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    // 新增：模拟时间命令
    fSimulationTimeCmd = new G4UIcmdWithADouble("/gun/simulationTime", this);
    fSimulationTimeCmd->SetGuidance("Set simulation time in seconds (for activity mode)");
    fSimulationTimeCmd->SetParameterName("time", true);
    fSimulationTimeCmd->SetDefaultValue(1.0);
    fSimulationTimeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete fCs137ActivityCmd;
    delete fTestModeCmd;
    delete fActivityModeCmd;  // 新增
    delete fSimulationTimeCmd;  // 新增
    delete fGunDir;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fCs137ActivityCmd) {
        G4double activity = fCs137ActivityCmd->GetNewDoubleValue(newValue);
        fAction->SetCs137Activity(activity);
    }
    else if (command == fTestModeCmd) {
        G4bool testMode = fTestModeCmd->GetNewBoolValue(newValue);
        fAction->SetTestMode(testMode);
    }
    else if (command == fActivityModeCmd) {  // 新增
        G4bool activityMode = fActivityModeCmd->GetNewBoolValue(newValue);
        fAction->SetActivityMode(activityMode);
    }
    else if (command == fSimulationTimeCmd) {  // 新增
        G4double time = fSimulationTimeCmd->GetNewDoubleValue(newValue);
        fAction->SetSimulationTime(time);
    }
}
