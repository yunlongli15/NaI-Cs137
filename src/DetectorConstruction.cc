#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"

DetectorConstruction::DetectorConstruction()
{
    // 房间尺寸: 40平方米, 高度3米
    roomSizeX = 8.0 * m;  // 8m x 5m = 40m²
    roomSizeY = 5.0 * m;
    roomSizeZ = 3.0 * m;
    
    // NaI探测器尺寸: 3英寸直径 × 3英寸高度
    naiRadius = 3.81 * cm;  // 3英寸 = 7.62cm直径 → 半径3.81cm
    naiHeight = 7.62 * cm;  // 3英寸高度
    canThickness = 2.0 * mm;  // 铝壳厚度
}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    
    // 空气
    air = nist->FindOrBuildMaterial("G4_AIR");
    
    // 铝 (外壳)
    aluminum = nist->FindOrBuildMaterial("G4_Al");
    
    // NaI(Tl) 晶体
    G4Element* na = nist->FindOrBuildElement("Na");
    G4Element* i = nist->FindOrBuildElement("I");
    
    nai = new G4Material("NaI", 3.67 * g/cm3, 2);
    nai->AddElement(na, 1);
    nai->AddElement(i, 1);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();
    
    // 世界体积 - 充满空气的房间
    G4Box* worldSolid = new G4Box("World", roomSizeX/2, roomSizeY/2, roomSizeZ/2);
    G4LogicalVolume* worldLog = new G4LogicalVolume(worldSolid, air, "World");
    worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLog, "World", 0, false, 0);
    
    // NaI探测器铝外壳
    G4Tubs* canSolid = new G4Tubs("NaICan", 
                                  0, 
                                  naiRadius + canThickness, 
                                  naiHeight/2 + canThickness, 
                                  0, 360*deg);
    G4LogicalVolume* canLog = new G4LogicalVolume(canSolid, aluminum, "NaICan");
    
    // 将探测器放置在房间中心
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), canLog, "NaICan", worldLog, false, 0);
    
    // NaI晶体
    G4Tubs* naiSolid = new G4Tubs("NaICrystal", 
                                  0, 
                                  naiRadius, 
                                  naiHeight/2, 
                                  0, 360*deg);
    naiCrystalLog = new G4LogicalVolume(naiSolid, nai, "NaICrystal");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), naiCrystalLog, "NaICrystal", canLog, false, 0);
    
    return worldPhys;
}

void DetectorConstruction::ConstructSDandField()
{
    // 创建灵敏探测器
    G4MultiFunctionalDetector* naiDetector = new G4MultiFunctionalDetector("NaIDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(naiDetector);
    
    // 添加能量沉积记分器
    G4VPrimitiveScorer* energyDeposit = new G4PSEnergyDeposit("EnergyDeposit");
    naiDetector->RegisterPrimitive(energyDeposit);
    
    // 将灵敏探测器附加到NaI晶体逻辑体积
    SetSensitiveDetector("NaICrystal", naiDetector);
}
