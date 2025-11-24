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
    void SetTestMode(G4bool mode);
    void SetActivityMode(G4bool mode);  // 新增：活度模式
    void SetSimulationTime(G4double time);  // 新增：模拟时间
    
    G4double GetCs137Activity() const;
    G4int GetEventsForActivityMode();  // 新增：计算活度模式的事件数
    
private:
    G4ParticleGun* particleGun;
    G4double cs137Activity;
    G4double roomVolume;
    G4bool testMode;
    G4bool activityMode;  // 新增：活度模式标志
    G4double simulationTime;  // 新增：模拟时间
    PrimaryGeneratorMessenger* fMessenger;
    
    void GenerateCs137Decay(G4Event* event);
    void GenerateGamma662(G4Event* event);
    void GenerateTestGamma(G4Event* event);
    void GenerateActivityModeGamma(G4Event* event);  // 新增：活度模式生成函数
};

#endif
