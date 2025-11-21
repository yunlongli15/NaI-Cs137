#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"

class RunAction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fTotalEdep += edep; }
    void SetHitPosition(const G4ThreeVector& pos);  

private:
    G4double fTotalEdep;
    RunAction* fRunAction;
};

#endif
