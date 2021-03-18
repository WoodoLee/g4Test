#include "OTDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4UniformElectricField.hh"
#include "G4ElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_SpinEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4IntegrationDriver.hh"

#include "G4ChordFinder.hh"
#include "G4IntegrationDriver.hh"

#include "OTDCMessenger.hh"

OTDetectorConstruction::OTDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fField = new OTField();
  new OTDCMessenger(this, fField);
}

OTDetectorConstruction::~OTDetectorConstruction()
{
}

G4VPhysicalVolume* OTDetectorConstruction::Construct()
{
  G4double a, z, density;
  // Material
  G4NistManager* nist = G4NistManager::Instance();
  auto air = nist -> FindOrBuildMaterial("G4_AIR");
  auto vacuum = new G4Material("vacuum", 1.e-15*g/cm3, 1);
  vacuum -> AddMaterial(air, 1);

  new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
  auto detectorMaterial  = G4Material::GetMaterial("Lead");

  // World
  G4double world_size = 5000*mm;
  auto solid_world = new G4Box("World", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  auto logic_world = new G4LogicalVolume(solid_world, air, "World");
  auto physi_world = new G4PVPlacement(0, G4ThreeVector(), logic_world, "World", 0, false, 0, true);


  // Detector
  G4double detector_rMin = 0 *mm;
  G4double detector_rMax = 100 *mm;
  G4double detector_height = 5000*mm;
  //auto solid_detector = new G4Tubs("Detector", detector_rMin, detector_rMax, detector_height/2., 0, 2*CLHEP::pi);
  auto solid_detector = new G4Box("Detector", 50.*cm, 50.*cm, 50*cm);
  logic_detector = new G4LogicalVolume(solid_detector, detectorMaterial, "Detector");
  new G4PVPlacement(0, G4ThreeVector(), logic_detector, "Detector", logic_world, false, 1, true);


  // Visualization Attribute
  auto visat_world = new G4VisAttributes();
  visat_world -> SetColor(G4Colour::Green());
  visat_world -> SetForceWireframe(true);
  logic_world -> SetVisAttributes(visat_world);

  auto visat_detector = new G4VisAttributes();
  visat_detector -> SetColor(G4Colour(0.8,0.8,0.8,0.3));
  logic_detector -> SetVisAttributes(visat_detector);

  
  // G4double eZ = 1. * megavolt/um;
  // G4double eY = 1. * megavolt/um;
  // G4double eX = 1. * megavolt/um;
  // G4double bZ = 0.1 * tesla;
  // 
  // G4ElectricField* sField = new G4UniformElectricField(G4ThreeVector(eX,eY, eZ));
  // //G4UniformMagField* sField = new G4UniformMagField(G4ThreeVector(0.,0.,bZ));
  // G4FieldManager* fieldManager = new G4FieldManager(sField);
  // //physi_world -> GetLogicalVolume() -> SetFieldManager(fieldMgr, false);
 
  // //fieldManager -> SetDetectorField(sField);


  //physi_world -> GetLogicalVolume() -> SetFieldManager(fieldManager, true);


  return physi_world;
}

void OTDetectorConstruction::SetGlobalField()
{
  pEquation = new G4EqMagElectricField(fField);

  G4int nvar = 8;

  // Create the Runge-Kutta 'stepper' using the efficient 'DoPri5' method
  auto pStepper = new G4DormandPrince745( pEquation, nvar );

  // Get the global field manager
  auto fieldManager= G4TransportationManager::GetTransportationManager()->
       GetFieldManager();
  // Set this field to the global field manager
  fieldManager->SetDetectorField( fField );

  G4double minStep     = 0.010*mm ; // minimal step of 10 microns

  // The driver will ensure that integration is control to give
  //   acceptable integration error
  auto pIntgrationDriver =
     new G4IntegrationDriver<G4DormandPrince745>(minStep,
                                                 pStepper,
                                                 nvar);

  pChordFinder = new G4ChordFinder(pIntgrationDriver);
  fieldManager->SetChordFinder( pChordFinder );
}

void OTDetectorConstruction::SetLocalField()
{
  auto fieldManager = new G4FieldManager();
  fieldManager -> SetDetectorField(fField);
  fieldManager -> CreateChordFinder((G4MagneticField *) fField);
  logic_detector -> SetFieldManager(fieldManager, true);
}
