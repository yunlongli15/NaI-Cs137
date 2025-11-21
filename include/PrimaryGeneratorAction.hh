#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
    
    void SetCs137Activity(G4double activity);
    void SetTestMode(G4bool mode);  // 添加测试模式设置
    G4double GetCs137Activity() const;
    
private:
    G4ParticleGun* particleGun;
    G4double cs137Activity;
    G4double roomVolume;
    G4bool testMode;  // 测试模式标志
    PrimaryGeneratorMessenger* fMessenger;
    
    void GenerateCs137Decay(G4Event* event);
    void GenerateGamma662(G4Event* event);
    void GenerateTestGamma(G4Event* event);  // 测试模式生成函数
};

#endif
