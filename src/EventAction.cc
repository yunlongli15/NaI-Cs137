#include "EventAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include <fstream>

// 添加全局变量来存储击中位置
namespace {
    G4ThreeVector gHitPosition;
}

EventAction::EventAction(RunAction* runAction)
 : fTotalEdep(0.),
   fRunAction(runAction)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fTotalEdep = 0.;
    gHitPosition = G4ThreeVector(0, 0, 0);
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    auto analysisManager = G4AnalysisManager::Instance();
    
    if (fTotalEdep > 0.) {
        // 填充能谱直方图
        analysisManager->FillH1(0, fTotalEdep);  // 全范围能谱
        analysisManager->FillH1(1, fTotalEdep);  // 放大区域能谱
        
        // 填充详细信息的Ntuple
        analysisManager->FillNtupleDColumn(0, fTotalEdep);      // 能量
        analysisManager->FillNtupleDColumn(1, event->GetEventID()); // 事件ID
        analysisManager->FillNtupleDColumn(2, gHitPosition.x());    // X位置
        analysisManager->FillNtupleDColumn(3, gHitPosition.y());    // Y位置  
        analysisManager->FillNtupleDColumn(4, gHitPosition.z());    // Z位置
        analysisManager->AddNtupleRow();
    }
    
    if (event->GetEventID() % 1000 == 0) {
        G4cout << "Event " << event->GetEventID() 
               << " - Energy deposit: " << fTotalEdep/keV << " keV" << G4endl;
    }
    
    // 累积能量沉积到RunAction
    if (fRunAction && fTotalEdep > 0.) {
        fRunAction->AddEnergyDeposit(fTotalEdep);
    }
}

// 添加设置击中位置的方法
void EventAction::SetHitPosition(const G4ThreeVector& pos)
{
    gHitPosition = pos;
}
