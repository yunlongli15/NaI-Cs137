#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : cs137Activity(1.0),
   roomVolume(261.8),  // 2/3*pi*5^3 = 261.8 m³
   testMode(false),
   activityMode(false),  // 默认关闭活度模式
   simulationTime(1.0)   // 默认1秒
{
    particleGun = new G4ParticleGun(1);
    fMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fMessenger;
    delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    if (testMode) {
        GenerateTestGamma(event);      // 测试模式：固定位置
    } else if (activityMode) {
        GenerateActivityModeGamma(event); // 活度模式：基于活度和时间
    } else {
        GenerateCs137Decay(event);     // 正常模式：随机位置
    }
}

void PrimaryGeneratorAction::GenerateCs137Decay(G4Event* event)
{
    GenerateGamma662(event);
}

// 在半球拱顶房间内随机生成粒子
void PrimaryGeneratorAction::GenerateGamma662(G4Event* event)
{
    G4ParticleDefinition* gamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    
    G4double roomRadius = 5.0 * m;
    G4double floorZ = - 0.1;  // 地面位置
    
    // 在半球拱顶房间体积内均匀生成粒子
    G4double x, y, z;
    G4bool positionValid = false;
    
    while (!positionValid) {
        // 在包围盒内随机生成
        x = (2 * G4UniformRand() - 1) * roomRadius;
        y = (2 * G4UniformRand() - 1) * roomRadius;
        z = G4UniformRand() * (roomRadius) + floorZ;
        
        // 检查是否在房间体积内
        G4double r_xy = sqrt(x*x + y*y);
        
        // 在半球拱顶部分
        G4double domeZ = z - (floorZ);
        if (r_xy*r_xy + domeZ*domeZ <= roomRadius*roomRadius) {
            positionValid = true;
        }
        
        // 确保在地面以上
        if (z < floorZ + 0.01) {
            positionValid = false;
        }
    }
    
    // 随机方向（4π立体角）
    G4double dirTheta = acos(1 - 2 * G4UniformRand());
    G4double dirPhi = 2.0 * M_PI * G4UniformRand();
    
    G4double dx = sin(dirTheta) * cos(dirPhi);
    G4double dy = sin(dirTheta) * sin(dirPhi);
    G4double dz = cos(dirTheta);
    
    G4ThreeVector direction(dx, dy, dz);
    
    particleGun->SetParticleDefinition(gamma);
    particleGun->SetParticleEnergy(662 * keV);
    particleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    particleGun->SetParticleMomentumDirection(direction);
    particleGun->GeneratePrimaryVertex(event);
    
    // 输出调试信息
    if (event->GetEventID() % 100 == 0) {
        G4cout << "Event " << event->GetEventID() 
               << ": Gamma at (" << x/m << ", " << y/m << ", " << z/m 
               << ") m" << G4endl;
    }
}

// 测试模式：固定位置直接射向探测器（保持不变）
void PrimaryGeneratorAction::GenerateTestGamma(G4Event* event)
{
    G4ParticleDefinition* gamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    
    // 固定位置：在探测器正前方1米处
    G4double x = 0.0 * m;
    G4double y = 0.0 * m;
    G4double z = 1.0 * m;  // 在探测器正前方
    
    // 固定方向：直接射向探测器中心
    G4ThreeVector direction(0, 0, -1);  // 指向原点（探测器位置）
    
    particleGun->SetParticleDefinition(gamma);
    particleGun->SetParticleEnergy(662 * keV);
    particleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    particleGun->SetParticleMomentumDirection(direction);
    particleGun->GeneratePrimaryVertex(event);
    
    // 在测试模式下输出信息
    if (event->GetEventID() % 100 == 0) {
        G4cout << "Test mode - Gamma from (" << x/m << ", " << y/m << ", " << z/m 
               << ") m directed to detector" << G4endl;
    }
}

// 新增：活度模式生成函数
void PrimaryGeneratorAction::GenerateActivityModeGamma(G4Event* event)
{
    G4ParticleDefinition* gamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    
    // 活度模式：在房间内随机位置（与正常模式相同的位置分布）
    G4double x = (G4UniformRand() - 0.5) * 8.0 * m;
    G4double y = (G4UniformRand() - 0.5) * 5.0 * m; 
    G4double z = (G4UniformRand() - 0.5) * 3.0 * m;
    
    // 随机方向（与正常模式相同的方向分布）
    G4double phi = 2.0 * M_PI * G4UniformRand();
    G4double cosTheta = 2.0 * G4UniformRand() - 1.0;
    G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    
    G4ThreeVector direction(sinTheta * std::cos(phi), 
                           sinTheta * std::sin(phi), 
                           cosTheta);
    
    particleGun->SetParticleDefinition(gamma);
    particleGun->SetParticleEnergy(662 * keV);
    particleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    particleGun->SetParticleMomentumDirection(direction);
    particleGun->GeneratePrimaryVertex(event);
    
    // 在活度模式下输出信息
    if (event->GetEventID() % 10000 == 0) {
        G4cout << "Activity mode - Event " << event->GetEventID() 
               << " at (" << x/m << ", " << y/m << ", " << z/m << ") m" << G4endl;
    }
}

// 新增：计算活度模式需要的事件数
G4int PrimaryGeneratorAction::GetEventsForActivityMode()
{
    if (!activityMode) {
        return 0;
    }
    
    G4double totalActivity = cs137Activity * roomVolume;  // 总活度 (Bq)
    G4double meanEvents = totalActivity * simulationTime; // 平均事件数
    
    // 使用泊松分布生成随机事件数
    G4int nEvents = CLHEP::RandPoisson::shoot(meanEvents);
    
    // 确保至少有一个事件
    if (nEvents <= 0) {
        nEvents = 1;
    }
    
    G4cout << "=== ACTIVITY MODE CALCULATION ===" << G4endl;
    G4cout << "Activity concentration: " << cs137Activity << " Bq/m³" << G4endl;
    G4cout << "Room volume: " << roomVolume << " m³" << G4endl;
    G4cout << "Total activity: " << totalActivity << " Bq" << G4endl;
    G4cout << "Simulation time: " << simulationTime << " s" << G4endl;
    G4cout << "Mean expected events: " << meanEvents << G4endl;
    G4cout << "Poisson sampled events: " << nEvents << G4endl;
    G4cout << "Event rate: " << nEvents/simulationTime << " events/s" << G4endl;
    G4cout << "==================================" << G4endl;
    
    return nEvents;
}

void PrimaryGeneratorAction::SetCs137Activity(G4double activity) 
{ 
    cs137Activity = activity; 
    G4double totalActivity = cs137Activity * roomVolume;
    G4cout << "Cs-137 activity set to: " << cs137Activity 
           << " Bq/m3, Total: " << totalActivity << " Bq" << G4endl;
}

void PrimaryGeneratorAction::SetTestMode(G4bool mode)
{
    testMode = mode;
    if (testMode) {
        activityMode = false;  // 确保互斥
        G4cout << "=== TEST MODE ACTIVATED ===" << G4endl;
        G4cout << "Gamma source fixed at (0, 0, 1) m" << G4endl;
        G4cout << "Directly shooting toward detector" << G4endl;
    } else {
        G4cout << "=== NORMAL MODE ===" << G4endl;
    }
}

// 新增：设置活度模式
void PrimaryGeneratorAction::SetActivityMode(G4bool mode)
{
    activityMode = mode;
    if (activityMode) {
        testMode = false;  // 确保互斥
        G4cout << "=== ACTIVITY MODE ACTIVATED ===" << G4endl;
        G4cout << "Gamma sources distributed randomly in room" << G4endl;
        G4cout << "Event count based on activity and simulation time" << G4endl;
        G4cout << "Current settings: " << cs137Activity << " Bq/m³, " 
               << simulationTime << " s" << G4endl;
    } else {
        G4cout << "Activity mode deactivated" << G4endl;
    }
}

// 新增：设置模拟时间
void PrimaryGeneratorAction::SetSimulationTime(G4double time)
{
    simulationTime = time;
    G4cout << "Simulation time set to: " << simulationTime << " seconds" << G4endl;
}

G4double PrimaryGeneratorAction::GetCs137Activity() const 
{ 
    return cs137Activity; 
}
