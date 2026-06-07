// RunAction.hh
#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"

class PrimaryGeneratorAction;

class RunAction : public G4UserRunAction {
public:
    explicit RunAction(PrimaryGeneratorAction* pga) : fPGA(pga) {}
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

private:
    PrimaryGeneratorAction* fPGA;
};

#endif // RUN_ACTION_HH
