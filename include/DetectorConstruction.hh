#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
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
    
    // 几何体
    G4VPhysicalVolume* worldPhys;
    G4LogicalVolume* naiCrystalLog;
    
    // 探测器尺寸
    G4double roomSizeX, roomSizeY, roomSizeZ;
    G4double naiRadius, naiHeight;
    G4double canThickness;
};

#endif
