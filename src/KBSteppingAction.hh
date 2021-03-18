#ifndef KBSTEPPINGACTION_HH__
#define KBSTEPPINGACTION_HH__

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "globals.hh"

class KBSteppingAction : public G4UserSteppingAction
{
  public:
    KBSteppingAction();
    virtual ~KBSteppingAction() {}

    virtual void UserSteppingAction(const G4Step*);
};

#endif
