#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trap.hh"
#include "G4VisAttributes.hh"

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // Materials
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *W = nist->FindOrBuildMaterial("G4_W"); // tungsten (74)
    G4Material *Ta = nist->FindOrBuildMaterial("G4_Ta"); // tantalum (73)
    G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu"); // copper (29)
    G4Element *elH = nist->FindOrBuildElement("H");
    G4Element *elC = nist->FindOrBuildElement("C");
    G4Element *elN = nist->FindOrBuildElement("N");
    G4Element *elO = nist->FindOrBuildElement("O");
    G4Material *Kapton = new G4Material("Kapton", 1.42 * g / cm3, 4);
    Kapton->AddElement(elH, 22);
    Kapton->AddElement(elC, 10);
    Kapton->AddElement(elN, 2);
    Kapton->AddElement(elO, 5);
    G4Material *Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
    G4Element *elW = nist->FindOrBuildElement("W");
    G4Element *elNi = nist->FindOrBuildElement("Ni");
    G4Element *elFe = nist->FindOrBuildElement("Fe");
    G4Material *tungstenAlloy = new G4Material("tungsten alloy", 18. * g / cm3, 3);
    tungstenAlloy->AddElement(elW, 90);
    tungstenAlloy->AddElement(elNi, 7);
    tungstenAlloy->AddElement(elFe, 3);

    // World
    G4double world_sizeXY = 5.*m;
    G4double world_sizeZ = 5.*m;
    auto solidWorld = new G4Box("World", .5*world_sizeXY, .5*world_sizeXY, .5*world_sizeZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);
    // no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, overlaps checking
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // Primary Collimator
    G4double PC_RMin = 3.8*cm;
    G4double PC_RMax = 37.72*cm;
    G4double PC_sizeZ = 76.2*cm;
    G4double PC_SPhi = 0.*deg;
    G4double PC_DPhi = 360.*deg;
    auto PC_pos = G4ThreeVector(0., 0., 1.9*m);
    auto solidPC = new G4Tubs("Primary Collimator", PC_RMin, PC_RMax, .5*PC_sizeZ, PC_SPhi, PC_DPhi);
    auto logicPC = new G4LogicalVolume(solidPC, W, "Primary Collimator");
    new G4PVPlacement(nullptr, PC_pos, logicPC, "Primary Collimator", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes PCVisAtt(G4Colour::Magenta());
    PCVisAtt.SetForceSolid(true);
    logicPC->SetVisAttributes(PCVisAtt);
    
    // Target
    G4double target_sizeXY = 1.2*PC_RMax;
    G4double target_sizeZ = .2*cm;
    auto target_pos = G4ThreeVector(0., 0., 2.4*m);
    auto solidTarget = new G4Box("Target", .5*target_sizeXY, .5*target_sizeXY, .5*target_sizeZ);
    auto logicTarget = new G4LogicalVolume(solidTarget, Ta, "Target");
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
    auto FF_pos = G4ThreeVector(0., 0., 1.4*m);
    auto solidFF = new G4Cons("Flattening Filter", FF_Rmin1, FF_Rmax1, FF_Rmin2, FF_Rmax2, .5*FF_Dz, FF_SPhi, FF_DPhi);
    auto logicFF = new G4LogicalVolume(solidFF, Cu, "Flattening Filter");
    new G4PVPlacement(nullptr, FF_pos, logicFF, "Flattening Filter", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes FFVisAtt(G4Colour::Grey());
    FFVisAtt.SetForceSolid(true);
    logicFF->SetVisAttributes(FFVisAtt);

    // Ion Chamber
    G4double IC_RMin = 0.;
    G4double IC_RMax = 35 * cm;
    G4double IC_sizeZ = 9. * cm;
    G4double IC_SPhi = 0. * deg;
    G4double IC_DPhi = 360. * deg;
    auto IC_pos = G4ThreeVector(0., 0., 1.2 * m);
    auto solidIC = new G4Tubs("Ion Chamber", IC_RMin, IC_RMax, .5 * IC_sizeZ, IC_SPhi, IC_DPhi);
    auto logicIC = new G4LogicalVolume(solidIC, air, "Ion Chamber");
    new G4PVPlacement(nullptr, IC_pos, logicIC, "Ion Chamber", logicWorld, false, 0, checkOverlaps);
    logicIC->SetVisAttributes(G4VisAttributes::GetInvisible());
    G4double Kapton_thickness = 25. * um;
    auto *solidICRep = new G4Tubs("Ion Chamber Rep", IC_RMin, IC_RMax, .5 * Kapton_thickness, 0. * deg, 360. * deg);
    G4LogicalVolume *logicICRep = new G4LogicalVolume(solidICRep, Kapton, "Ion Chamber Rep");
    new G4PVReplica("Ion Chamber Rep", logicICRep, logicIC, kZAxis, 6, 1.5 * cm, 0.);
    G4VisAttributes ICVisAtt(G4Colour::Yellow());
    ICVisAtt.SetForceWireframe(true);
    logicICRep->SetVisAttributes(ICVisAtt);

    // Mirror
    G4double mirror_sizeXY = 22.098*2 * cm;
    G4double mirror_sizeZ = .0508 * cm;
    auto mirror_pos = G4ThreeVector(0., 0., 1. * m);
    G4RotationMatrix *mirrorRot = new G4RotationMatrix();
    mirrorRot->rotateX(36. * deg);
    auto solidMirror = new G4Box("Mirror", .5 * mirror_sizeXY, .5 * mirror_sizeXY, .5 * mirror_sizeZ);
    auto logicMirror = new G4LogicalVolume(solidMirror, Mylar, "Mirror");
    new G4PVPlacement(mirrorRot, mirror_pos, logicMirror, "Mirror", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes mirrorVisAtt(G4Colour::Grey());
    mirrorVisAtt.SetForceSolid(true);
    logicMirror->SetVisAttributes(mirrorVisAtt);

    // Jaws
    G4double pZ = 60.*cm, pY = 30.*cm, pX = 30.*cm, pLTX = 20.*cm;
    auto jaw_pos1 = G4ThreeVector(-20.*cm, 0., .6* m);
    G4RotationMatrix *jawRot1 = new G4RotationMatrix();
    jawRot1->rotateX(pi/2.);
    auto solidJaw1 = new G4Trap("Jaw1", pZ, pY, pX, pLTX);
    auto logicJaw1 = new G4LogicalVolume(solidJaw1, tungstenAlloy, "Jaw1");
    new G4PVPlacement(jawRot1, jaw_pos1, logicJaw1, "Jaw1", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt1(G4Colour::Red());
    jawVisAtt1.SetForceSolid(true);
    logicJaw1->SetVisAttributes(jawVisAtt1);

    auto jaw_pos2 = G4ThreeVector(20. * cm, 0., .6 * m);
    G4RotationMatrix *jawRot2 = new G4RotationMatrix();
    jawRot2->rotateX(pi / 2.);
    jawRot2->rotateY(pi);
    auto solidJaw2 = new G4Trap("Jaw2", pZ, pY, pX, pLTX);
    auto logicJaw2 = new G4LogicalVolume(solidJaw2, tungstenAlloy, "Jaw2");
    new G4PVPlacement(jawRot2, jaw_pos2, logicJaw2, "Jaw2", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt2(G4Colour::Magenta());
    jawVisAtt2.SetForceSolid(true);
    logicJaw2->SetVisAttributes(jawVisAtt2);

    auto jaw_pos3 = G4ThreeVector(0, -20.*cm, .25 * m);
    G4RotationMatrix *jawRot3 = new G4RotationMatrix();
    jawRot3->rotateX(pi / 2.);
    jawRot3->rotateY(pi/2.);
    auto solidJaw3 = new G4Trap("Jaw3", pZ, pY, pX, pLTX);
    auto logicJaw3 = new G4LogicalVolume(solidJaw3, tungstenAlloy, "Jaw3");
    new G4PVPlacement(jawRot3, jaw_pos3, logicJaw3, "Jaw3", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt3(G4Colour::Blue());
    jawVisAtt3.SetForceSolid(true);
    logicJaw3->SetVisAttributes(jawVisAtt3);

    auto jaw_pos4 = G4ThreeVector(0, 20. * cm, .25 * m);
    G4RotationMatrix *jawRot4 = new G4RotationMatrix();
    jawRot4->rotateX(pi / 2.);
    jawRot4->rotateY(-pi / 2.);
    auto solidJaw4 = new G4Trap("Jaw4", pZ, pY, pX, pLTX);
    auto logicJaw4 = new G4LogicalVolume(solidJaw4, tungstenAlloy, "Jaw4");
    new G4PVPlacement(jawRot4, jaw_pos4, logicJaw4, "Jaw4", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt4(G4Colour::Cyan());
    jawVisAtt4.SetForceSolid(true);
    logicJaw4->SetVisAttributes(jawVisAtt4);

    // Multi-leaf Collimator
    G4double MLC_xy = 1.5*m;
    G4double MLC_z = .4*m;
    auto MLC_tlate = G4ThreeVector(0., 0., -.2*m);
    auto solidMLC = new G4Box("MLC", .5 * MLC_xy, .5 * MLC_xy, .5 * MLC_z);
    auto logicMLC = new G4LogicalVolume(solidMLC, air, "MLC");
    new G4PVPlacement(nullptr, MLC_tlate, logicMLC, "MLC", logicWorld, false, 0, checkOverlaps);
    auto solidMLCRep = new G4Box("MLC Rep", .5 * MLC_xy, .25 * MLC_xy, .5 * MLC_z);
    auto logicMLCRep = new G4LogicalVolume(solidMLCRep, air, "MLC Rep");
    new G4PVReplica("MLC Rep", logicMLCRep, logicMLC, kYAxis, 2, .5 * MLC_xy);
    auto solidMLCRep2 = new G4Box("MLC Rep2", .3 * MLC_xy, .15 * MLC_xy, .3 * MLC_z);
    auto logicMLCRep2 = new G4LogicalVolume(solidMLCRep2, tungstenAlloy, "MLC Rep2");
    auto MLC_tlate2 = G4ThreeVector(0., 0., 0.);
    new G4PVPlacement(nullptr, MLC_tlate2, logicMLCRep2, "MLC Rep2", logicMLCRep, false, 0, checkOverlaps);
    G4VisAttributes MLCVisAtt(G4Colour::Magenta());
    MLCVisAtt.SetForceSolid(true);
    logicMLCRep2->SetVisAttributes(MLCVisAtt);

    // Wedge Filter
    G4double WF_z = 1. * m, WF_y = 1.*m, WF_x = 10.*cm, WF_ltx = 1. * cm;
    auto WF_tlate = G4ThreeVector(0., 0., -.6 * m);
    G4RotationMatrix *WF_rot = new G4RotationMatrix();
    WF_rot->rotateY(pi / 2.);
    auto solidWF = new G4Trap("Wedge Filter", WF_z, WF_y, WF_x, WF_ltx);
    auto logicWF = new G4LogicalVolume(solidWF, tungstenAlloy, "Wedge Filter");
    new G4PVPlacement(WF_rot, WF_tlate, logicWF, "Wedge Filter", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes WFVisAtt(G4Colour::Blue());
    WFVisAtt.SetForceSolid(true);
    logicWF->SetVisAttributes(WFVisAtt);

    return physWorld;
}
