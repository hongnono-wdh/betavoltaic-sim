// EventAction.cc
#include "EventAction.hh"
#include "Analysis.hh"
#include "G4Event.hh"

void EventAction::BeginOfEventAction(const G4Event* evt) {
    fEventID = evt->GetEventID();
    fRecordTrack = Analysis::Instance()->ShouldRecordTrack(fEventID);
    fIncidentCounted = false;
    fBackCounted = false;
}

void EventAction::EndOfEventAction(const G4Event*) {}
