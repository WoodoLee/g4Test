#ifndef OTDETECTORCONSTRUCTION_HH
#define OTDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "OTField.hh"
#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4DormandPrince745.hh"
#include "G4ChordFinder.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;

class OTDetectorConstruction : public G4VUserDetectorConstruction
{
    G4ElectricField*        pEMfield;
  G4EqMagElectricField*   pEquation;
  G4ChordFinder*          pChordFinder ;
  public:
    OTDetectorConstruction();
    virtual ~OTDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    //void SetDetectorRMax(G4double rmax) { detector_rMax = rmax; }

    void SetGlobalField();
    void SetLocalField();

  private:
    OTField *fField;
    G4LogicalVolume *logic_detector;
    //G4double detector_rMax = 100*mm;
};

#endif
