#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
 : fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // 获取步长所在的物理体积
    G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
    
    if (!preVolume) return;
    
    // 检查是否在NaI晶体中
    G4LogicalVolume* preLogical = preVolume->GetLogicalVolume();
    G4String volumeName = preLogical->GetName();
    
    if (volumeName == "NaICrystal") {
        // 在NaI晶体中的能量沉积
        G4double edep = step->GetTotalEnergyDeposit();
        if (edep > 0.) {
            fEventAction->AddEdep(edep);
            // 记录击中位置
            G4ThreeVector hitPosition = step->GetPreStepPoint()->GetPosition();
            fEventAction->SetHitPosition(hitPosition);
        }
    }
    
    // 可选：记录粒子信息用于调试
    G4Track* track = step->GetTrack();
    G4double energy = track->GetKineticEnergy();
    
    // 如果能量很低，停止跟踪以节省计算时间
    if (energy < 1.0 * keV) {
        track->SetTrackStatus(fStopAndKill);
    }
}
