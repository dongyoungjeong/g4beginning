#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;
    
    // World
    G4double world_sizeXY = 5.*m;
    G4double world_sizeZ = 5.*m;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
    auto solidWorld = new G4Box("World", .5*world_sizeXY, .5*world_sizeXY, .5*world_sizeZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);
    // no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, overlaps checking
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // Primary Collimator
    G4double PC_RMin = 3.8*cm;
    G4double PC_RMax = 37.72*cm;
    G4double PC_sizeZ = 76.2*cm;
    G4double PC_SPhi = 0.*deg;
    G4double PC_DPhi = 360.*deg;
    G4Material* PC_mat = nist->FindOrBuildMaterial("G4_W"); // tungsten (74)
    auto PC_pos = G4ThreeVector(0., 0., 1.9*m);
    auto solidPC = new G4Tubs("Primary Collimator", PC_RMin, PC_RMax, .5*PC_sizeZ, PC_SPhi, PC_DPhi);
    auto logicPC = new G4LogicalVolume(solidPC, PC_mat, "Primary Collimator");
    new G4PVPlacement(nullptr, PC_pos, logicPC, "Primary Collimator", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes PCVisAtt(G4Colour::Magenta());
    PCVisAtt.SetForceSolid(true);
    logicPC->SetVisAttributes(PCVisAtt);
    
    // Target
    G4double target_sizeXY = PC_RMax; // approximation
    G4double target_sizeZ = 10.*cm; // approximation
    G4Material* target_mat = nist->FindOrBuildMaterial("G4_Ta"); // tantalum (73)
    auto target_pos = G4ThreeVector(0., 0., 2.4*m);
    auto solidTarget = new G4Box("Target", .5*target_sizeXY, .5*target_sizeXY, .5*target_sizeZ);
    auto logicTarget = new G4LogicalVolume(solidTarget, target_mat, "Target");
    new G4PVPlacement(nullptr, target_pos, logicTarget, "Target", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes targetVisAtt(G4Colour::Blue());
    targetVisAtt.SetForceSolid(true);
    logicTarget->SetVisAttributes(targetVisAtt);
    
    // Flattening Filter
    G4double FF_Rmin1 = 0.*cm;
    G4double FF_Rmax1 = .5*PC_RMax;
    G4double FF_Rmin2 = 0.*cm;
    G4double FF_Rmax2 = 1.*cm;
    G4double FF_Dz = 10.*cm;
    G4double FF_SPhi = 0.*deg;
    G4double FF_DPhi = 360.*deg;
    G4Material* FF_mat = nist->FindOrBuildMaterial("G4_Cu"); // copper (29)
    auto FF_pos = G4ThreeVector(0., 0., 1.4*m);
    auto solidFF = new G4Cons("Flattening Filter", FF_Rmin1, FF_Rmax1, FF_Rmin2, FF_Rmax2, .5*FF_Dz, FF_SPhi, FF_DPhi);
    auto logicFF = new G4LogicalVolume(solidFF, FF_mat, "Flattening Filter");
    new G4PVPlacement(nullptr, FF_pos, logicFF, "Flattening Filter", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes FFVisAtt(G4Colour::Grey());
    FFVisAtt.SetForceSolid(true);
    logicFF->SetVisAttributes(FFVisAtt);

//    // Shape 1
//    G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
//    G4ThreeVector pos1 = G4ThreeVector(0., 2.*cm, -7.*cm);
//
//    // Conical section shape
//    G4double shape1_rmina = 0.*cm;
//    G4double shape1_rmaxa = 2.*cm;
//    G4double shape1_rminb = 0.*cm;
//    G4double shape1_rmaxb = 4.*cm;
//    G4double shape1_hz = 3.*cm;
//    G4double shape1_phimin = 0.*deg;
//    G4double shape1_phimax = 360.*deg;
//    auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz, shape1_phimin, shape1_phimax);
//
//    auto logicShape1 = new G4LogicalVolume(solidShape1, shape1_mat, "Shape1");
//
//    new G4PVPlacement(nullptr, pos1, logicShape1, "Shape1", logicEnv, false, 0, checkOverlaps);
//
//    // Shape 2
//    G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
//    G4ThreeVector pos2 = G4ThreeVector(0, -1.*cm, 7.*cm);
//
//    // Trapezoid shape
//    G4double shape2_dxa = 12.*cm;
//    G4double shape2_dxb = 12.*cm;
//    G4double shape2_dya = 10.*cm;
//    G4double shape2_dyb = 16.*cm;
//    G4double shape2_dz = 6.*cm;
//    auto solidShape2 = new G4Trd("Shape2", .5*shape2_dxa, .5*shape2_dxb, .5*shape2_dya, .5*shape2_dyb, .5*shape2_dz);
//    auto logicShape2 = new G4LogicalVolume(solidShape2, shape2_mat, "Shape2");
//    new G4PVPlacement(nullptr, pos2, logicShape2, "Shape2", logicEnv, false, 0, checkOverlaps);
//    
//    fScoringVolume = logicShape2;
    
    return physWorld;
}
