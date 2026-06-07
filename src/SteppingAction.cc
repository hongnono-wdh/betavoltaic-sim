// SteppingAction.cc
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step* step) {
    auto* pre  = step->GetPreStepPoint();
    auto* post = step->GetPostStepPoint();

    // —— 入射能量(EDR 分母):primary β 首次下穿结构顶面 z=0 时记其动能 —— //
    if (!fEventAction->IncidentCounted() &&
        step->GetTrack()->GetParentID() == 0 &&
        pre->GetPosition().z() > 0.0 && post->GetPosition().z() <= 0.0) {
        Analysis::Instance()->AddIncidentEnergy(post->GetKineticEnergy() / keV);
        fEventAction->SetIncidentCounted();
    }

    // 当前所在物理体名（在 pre-step 体内发生沉积）
    auto* vol = pre->GetTouchableHandle()->GetVolume();
    const G4String name = vol ? vol->GetName() : G4String("");

    const G4double edepKeV = step->GetTotalEnergyDeposit() / keV;

    // 步中点深度 d = -z
    const G4ThreeVector mid = 0.5 * (pre->GetPosition() + post->GetPosition());
    const G4double zUmMid = mid.z() / um;
    const G4double depthUm = -zUmMid;
    const G4double xUm = mid.x() / um;

    // —— EDD/EDR：只统计结构(管壁/实心) 与 电解液 —— //
    if (edepKeV > 0.0) {
        if (name == "structure")
            Analysis::Instance()->AddEdep(depthUm, edepKeV, /*isFill=*/false);
        else if (name == "fill")
            Analysis::Instance()->AddEdep(depthUm, edepKeV, /*isFill=*/true);
        // 在 source/substrate/world 的沉积视为损失（自吸收/穿透），不计入。
    }

    // —— 轨迹与沉积点（仅前 nTracks 事件）—— //
    if (fEventAction->RecordTrack()) {
        const G4int id = fEventAction->EventID();
        const G4double zUmPost = post->GetPosition().z() / um;
        const G4double xUmPost = post->GetPosition().x() / um;
        Analysis::Instance()->AddTrackPoint(id, xUmPost, zUmPost);
        if (edepKeV > 0.0)
            Analysis::Instance()->AddEdepPoint(id, xUm, zUmMid, edepKeV);
    }
}
