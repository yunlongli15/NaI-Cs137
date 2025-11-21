#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include <fstream>

RunAction::RunAction()
 : totalEnergyDeposit(0.),
   numEvents(0)
{
    // 创建分析管理器
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // 使用CSV格式
    analysisManager->SetDefaultFileType("csv");
    
    // 创建能谱直方图 - 重点在这里
    analysisManager->CreateH1("EnergySpectrum", "Gamma Energy Spectrum in NaI", 
                             1000, 0., 2000.*keV);  // 0-2000 keV, 1000通道
    
    analysisManager->CreateH1("EnergySpectrum_zoom", "Gamma Energy Spectrum (662 keV region)", 
                             200, 600.*keV, 800.*keV);  // 662 keV附近区域
    
    // 创建Ntuple存储详细信息
    analysisManager->CreateNtuple("GammaSpectrum", "Gamma Spectrum Data");
    analysisManager->CreateNtupleDColumn("EnergyDeposit");  // 能量沉积 (keV)
    analysisManager->CreateNtupleDColumn("EventID");        // 事件ID
    analysisManager->CreateNtupleDColumn("X");              // 击中位置X
    analysisManager->CreateNtupleDColumn("Y");              // 击中位置Y  
    analysisManager->CreateNtupleDColumn("Z");              // 击中位置Z
    analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
    // 析构函数实现
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
    
    // 重置计数器
    totalEnergyDeposit = 0.;
    numEvents = 0;
    
    // 打开输出文件
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("nai_simulation");  // CSV格式不需要指定扩展名
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;
    
    // 输出统计结果
    G4cout << G4endl
           << "==================== RUN SUMMARY ====================" << G4endl
           << " Number of events processed: " << nofEvents << G4endl
           << " Total events with energy deposit: " << numEvents << G4endl
           << " Total energy deposit in NaI: " 
           << G4BestUnit(totalEnergyDeposit, "Energy") << G4endl;
    
    if (numEvents > 0) {
        G4cout << " Average energy deposit per hit event: " 
               << G4BestUnit(totalEnergyDeposit/numEvents, "Energy") << G4endl;
    }
    
    G4cout << " Average energy deposit per primary: " 
           << G4BestUnit(totalEnergyDeposit/nofEvents, "Energy") << G4endl
           << " Detection efficiency: " << (G4double)numEvents/nofEvents * 100.0 << " %" << G4endl
           << "=====================================================" << G4endl;
    
    // 保存分析数据
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    
    // 生成专门的能谱数据文件
    GenerateSpectrumData();
}

void RunAction::GenerateSpectrumData()
{
    // 生成便于绘图的能谱数据
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // 获取直方图数据
    G4int histogramId = analysisManager->GetH1Id("EnergySpectrum");
    
    std::ofstream spectrumFile("gamma_spectrum_data.csv");
    if (spectrumFile.is_open()) {
        spectrumFile << "Channel,Energy_keV,Counts" << std::endl;
        
        // 假设有1000个通道
        for (G4int i = 0; i < 1000; i++) {
            G4double energy = i * 2.0; // 每个通道2 keV
            G4double counts = analysisManager->GetH1(histogramId)->bin_height(i);
            spectrumFile << i << "," << energy << "," << counts << std::endl;
        }
        spectrumFile.close();
        G4cout << "Gamma spectrum data saved to: gamma_spectrum_data.csv" << G4endl;
    }
    
    // 保存统计信息
    std::ofstream statsFile("simulation_stats.txt");
    if (statsFile.is_open()) {
        statsFile << "Simulation Statistics" << std::endl;
        statsFile << "====================" << std::endl;
        statsFile << "Total events: " << numEvents << std::endl;
        statsFile << "Events with energy deposit: " << numEvents << std::endl;
        statsFile << "Total energy deposit: " << totalEnergyDeposit/keV << " keV" << std::endl;
        statsFile << "Average energy per hit: " << (numEvents > 0 ? totalEnergyDeposit/numEvents/keV : 0) << " keV" << std::endl;
        statsFile << "Detection efficiency: " << (G4double)numEvents/numEvents * 100.0 << " %" << std::endl;
        
        // 添加能谱特征
        statsFile << "Spectrum Features:" << std::endl;
        statsFile << "- Cs-137 gamma peak: 662 keV" << std::endl;
        statsFile << "- Energy range: 0-2000 keV" << std::endl;
        statsFile << "- Number of channels: 1000" << std::endl;
        
        statsFile.close();
    }
}

void RunAction::AddEnergyDeposit(G4double edep)
{
    totalEnergyDeposit += edep;
    numEvents += 1;
}
