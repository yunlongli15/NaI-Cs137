#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : cs137Activity(1.0),
   roomVolume(120.0),
   testMode(false)  // 默认关闭测试模式
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
        GenerateTestGamma(event);  // 测试模式：固定位置
    } else {
        GenerateCs137Decay(event); // 正常模式：随机位置
    }
}

void PrimaryGeneratorAction::GenerateCs137Decay(G4Event* event)
{
    GenerateGamma662(event);
}

void PrimaryGeneratorAction::GenerateGamma662(G4Event* event)
{
    G4ParticleDefinition* gamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    
    // 正常模式：在房间内随机位置
    G4double x = (G4UniformRand() - 0.5) * 8.0 * m;
    G4double y = (G4UniformRand() - 0.5) * 5.0 * m; 
    G4double z = (G4UniformRand() - 0.5) * 3.0 * m;
    
    // 随机方向
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
}

// 测试模式：固定位置直接射向探测器
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
        G4cout << "=== TEST MODE ACTIVATED ===" << G4endl;
        G4cout << "Gamma source fixed at (0, 0, 1) m" << G4endl;
        G4cout << "Directly shooting toward detector" << G4endl;
    } else {
        G4cout << "=== NORMAL MODE ===" << G4endl;
    }
}

G4double PrimaryGeneratorAction::GetCs137Activity() const 
{ 
    return cs137Activity; 
}
