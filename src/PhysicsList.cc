// 在 PhysicsList.cc 中修改物理过程参数
#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmParameters.hh"

PhysicsList::PhysicsList()
{
    // 电磁相互作用 - 使用更精确的选项
    RegisterPhysics(new G4EmStandardPhysics_option4());
    
    // 粒子衰变
    RegisterPhysics(new G4DecayPhysics());
    
    // 放射性衰变
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    
    // 设置电磁过程的参数 - 调整以减少数值问题
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetDefaults();
    param->SetVerbose(0);
    param->SetMinEnergy(1*keV);        // 提高最小能量
    param->SetMaxEnergy(10*GeV);
    param->SetLowestElectronEnergy(1*keV);  // 提高电子最低能量
    param->SetNumberOfBinsPerDecade(10);    // 减少bin数
    param->SetMscStepLimitType(fUseDistanceToBoundary);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
    SetCutValue(0.5 * mm, "gamma");     // 提高γ射线截止距离
    SetCutValue(0.05 * mm, "e-");       // 提高电子截止距离  
    SetCutValue(0.05 * mm, "e+");       // 提高正电子截止距离
    SetCutValue(0.1 * mm, "proton");    // 质子截止距离
}
