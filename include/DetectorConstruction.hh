#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
private:
    void DefineMaterials();
    void SetupGeometry();
    
    // 材料
    G4Material* air;
    G4Material* nai;
    G4Material* aluminum;
    G4Material* concrete;  // 新增：地面材料
    
    // 几何体
    G4VPhysicalVolume* worldPhys;
    G4LogicalVolume* naiCrystalLog;
    
    // 探测器尺寸
    G4double naiRadius, naiHeight;
    G4double canThickness;
    
    // 房间尺寸 - 修改为半球形
    G4double roomRadius;    // 半球半径
    G4double floorLevel;    // 地面高度（探测器离地高度）
};

#endif
