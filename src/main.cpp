// 在main.cpp中添加对活度模式的支持
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

int main(int argc, char** argv)
{
    // 设置随机数种子
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);
    
    // 创建运行管理器
    G4RunManager* runManager = new G4RunManager;
    
    // 初始化几何
    runManager->SetUserInitialization(new DetectorConstruction);
    
    // 初始化物理过程
    runManager->SetUserInitialization(new PhysicsList);
    
    // 设置用户动作 - 注意正确的顺序
    // 先创建RunAction
    RunAction* runAction = new RunAction;
    runManager->SetUserAction(runAction);
    
    // 创建EventAction，传入RunAction指针
    EventAction* eventAction = new EventAction(runAction);
    runManager->SetUserAction(eventAction);
    
    // 创建SteppingAction，传入EventAction指针
    SteppingAction* steppingAction = new SteppingAction(eventAction);
    runManager->SetUserAction(steppingAction);
    
    // 创建PrimaryGeneratorAction
    PrimaryGeneratorAction* primaryGenerator = new PrimaryGeneratorAction;
    runManager->SetUserAction(primaryGenerator);
    
    // 初始化Geant4内核
    runManager->Initialize();
    
    // 可视化管理器
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // 获取UI管理器
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    
    if (argc == 1) {
        // 交互模式
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
    }
    else {
        // 批处理模式
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
        
        // 如果是活度模式，自动计算事件数
        G4int eventsForActivityMode = primaryGenerator->GetEventsForActivityMode();
        if (eventsForActivityMode > 0) {
            G4String runCommand = "/run/beamOn ";
            G4String eventNum = G4String(std::to_string(eventsForActivityMode));
            G4cout << "Activity mode: Running " << eventsForActivityMode << " events" << G4endl;
            UImanager->ApplyCommand(runCommand + eventNum);
        }
    }
    
    // 清理
    delete ui;
    delete visManager;
    delete runManager;
    
    return 0;
}
