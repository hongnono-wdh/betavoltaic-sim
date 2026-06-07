// ActionInitialization.cc
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::Build() const {
    auto* pga = new PrimaryGeneratorAction();
    SetUserAction(pga);
    SetUserAction(new RunAction(pga));

    auto* ea = new EventAction();
    SetUserAction(ea);
    SetUserAction(new SteppingAction(ea));
}
