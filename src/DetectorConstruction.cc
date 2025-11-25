#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PhysicalConstants.hh"

DetectorConstruction::DetectorConstruction()
{
    // 房间尺寸: 半径为5米的半球拱顶
    roomRadius = 5.0 * m;
    
    // 探测器离地面高度
    floorLevel = 1.0 * m;  // 离地面1米
    
    // NaI探测器尺寸
    naiRadius = 3.81 * cm;
    naiHeight = 7.62 * cm;
    canThickness = 2.0 * mm;
}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    
    air = nist->FindOrBuildMaterial("G4_AIR");
    aluminum = nist->FindOrBuildMaterial("G4_Al");
    concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
    
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
    
    // 世界体积：半球拱顶
    G4double worldRadius = roomRadius;
    
    // 创建半球拱顶部分
    G4Sphere* domeSolid = new G4Sphere("DomePart",
                                       0,
                                       roomRadius,
                                       0, 360*deg,     // phi范围
                                       0, 90*deg);     // theta范围：上半球
    
    G4LogicalVolume* worldLog = new G4LogicalVolume(domeSolid, air, "World");
    worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLog, "World", 0, false, 0);
    
    // 创建地面（在圆柱底部）
    G4double floorThickness = 20.0 * cm;
    G4Tubs* floorSolid = new G4Tubs("Floor", 
                                    0, 
                                    worldRadius, 
                                    floorThickness/2, 
                                    0, 360*deg);
    
    G4LogicalVolume* floorLog = new G4LogicalVolume(floorSolid, concrete, "Floor");
    G4double floorZ = - floorThickness/2;
    new G4PVPlacement(0, 
                     G4ThreeVector(0, 0, floorZ), 
                     floorLog, "Floor", worldLog, false, 0);
    
    // NaI探测器铝外壳
    G4Tubs* canSolid = new G4Tubs("NaICan", 
                                  0, 
                                  naiRadius + canThickness, 
                                  naiHeight/2 + canThickness, 
                                  0, 360*deg);
    G4LogicalVolume* canLog = new G4LogicalVolume(canSolid, aluminum, "NaICan");
    
    // 将探测器放置在房间中心离地面1米高处
    // 地面在 z = floorZ，所以探测器位置为：
    G4double detectorZ = floorZ + floorThickness/2 + floorLevel + (naiHeight/2 + canThickness);
    new G4PVPlacement(0, G4ThreeVector(0, 0, detectorZ), canLog, "NaICan", worldLog, false, 0);
    
    // NaI晶体
    G4Tubs* naiSolid = new G4Tubs("NaICrystal", 
                                  0, 
                                  naiRadius, 
                                  naiHeight/2, 
                                  0, 360*deg);
    naiCrystalLog = new G4LogicalVolume(naiSolid, nai, "NaICrystal");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), naiCrystalLog, "NaICrystal", canLog, false, 0);
    
    G4cout << "=== Geometry Summary ===" << G4endl;
    G4cout << "Room: Cylinder + Hemisphere dome, radius " << roomRadius/m << " m" << G4endl;
    G4cout << "Floor position (Z): " << floorZ/m << " m" << G4endl;
    G4cout << "Detector position (Z): " << detectorZ/m << " m" << G4endl;
    G4cout << "Detector height above floor: " << floorLevel/m << " m" << G4endl;
    G4cout << "=========================" << G4endl;
    
    return worldPhys;
}

void DetectorConstruction::ConstructSDandField()
{
    G4MultiFunctionalDetector* naiDetector = new G4MultiFunctionalDetector("NaIDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(naiDetector);
    
    G4VPrimitiveScorer* energyDeposit = new G4PSEnergyDeposit("EnergyDeposit");
    naiDetector->RegisterPrimitive(energyDeposit);
    
    SetSensitiveDetector("NaICrystal", naiDetector);
}
