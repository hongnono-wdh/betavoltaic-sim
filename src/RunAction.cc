// RunAction.cc
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4ios.hh"

void RunAction::BeginOfRunAction(const G4Run*) {
    // 用当前 prefix/nTracks/maxDepth 初始化并清零直方图
    if (fPGA) fPGA->ConfigureAnalysis();
    G4cout << "[RunAction] Run started." << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4cout << "[RunAction] Run ended. Events = "
           << run->GetNumberOfEvent() << G4endl;
    Analysis::Instance()->Write();
}
