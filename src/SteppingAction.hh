// SteppingAction.hh
// 每步：累加 EDD/EDR（按管壁/电解液分类）+ 记录轨迹/沉积点（前 nTracks 事件）。
#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction {
public:
    explicit SteppingAction(EventAction* ea) : fEventAction(ea) {}
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step* step) override;

private:
    EventAction* fEventAction;
};

#endif // STEPPING_ACTION_HH
