// 在 PhysicsList.cc 中添加更详细的物理过程
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
    
    // 放射性衰变 - 关键！用于Cs-137衰变链
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    
    // 设置电磁过程的参数
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetDefaults();
    param->SetVerbose(0);
    param->SetMinEnergy(100*eV);
    param->SetMaxEnergy(10*GeV);
    param->SetLowestElectronEnergy(100*eV);
    param->SetNumberOfBinsPerDecade(20);
    param->SetMscStepLimitType(fUseDistanceToBoundary);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
    SetCutValue(1.0 * mm, "gamma");     // γ射线截止距离
    SetCutValue(0.1 * mm, "e-");        // 电子截止距离  
    SetCutValue(0.1 * mm, "e+");        // 正电子截止距离
    SetCutValue(0.1 * mm, "proton");    // 质子截止距离
}
