#include "KBSteppingAction.hh"

#include "G4Event.hh"

KBSteppingAction::KBSteppingAction()
: G4UserSteppingAction()
{
}

void KBSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4cout << "Time is.." << step -> GetPreStepPoint() -> GetGlobalTime() << G4endl;
    
  G4ThreeVector pos = step -> GetPreStepPoint() -> GetPosition();

  G4cout << "Positions are.. (" << pos.x() << " , " << pos.y() << ", " << pos.z() << ")"<< G4endl;
}
