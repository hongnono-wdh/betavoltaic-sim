// SteppingAction.cc
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

void SteppingAction::UserSteppingAction(const G4Step* step) {
    auto* pre  = step->GetPreStepPoint();
    auto* post = step->GetPostStepPoint();

    // —— 入射能量(EDR 分母):primary β 首次下穿结构顶面 z=0 时记其动能 —— //
    if (!fEventAction->IncidentCounted() &&
        step->GetTrack()->GetParentID() == 0 &&
        pre->GetPosition().z() > 0.0 && post->GetPosition().z() <= 0.0) {
        Analysis::Instance()->AddIncidentEnergy(post->GetKineticEnergy() / keV);
        Analysis::Instance()->AddIncidentCount();
        fEventAction->SetIncidentCounted();
    }

    // —— 数量背散射计数:primary 进入后首次向上越过顶面(z=0)逃逸 = 一次背散射 —— //
    if (fEventAction->IncidentCounted() && !fEventAction->BackCounted() &&
        step->GetTrack()->GetParentID() == 0 &&
        pre->GetPosition().z() <= 0.0 && post->GetPosition().z() > 0.0 &&
        step->GetTrack()->GetMomentumDirection().z() > 0.0) {
        Analysis::Instance()->AddBackscatterCount();
        fEventAction->SetBackCounted();
    }

    // —— 顶面再俘获:β 向上越过结构顶面(z=0)逃逸时,以概率 R 反射回结构 —— //
    // 等效模拟上方电极(⁶³Ni/Ni 阴极)对背散射 β 的反射;R 由 /ana/reflectR 设。
    const G4double R = Analysis::Instance()->ReflectR();
    if (R > 0.0 && pre->GetPosition().z() <= 0.0 && post->GetPosition().z() > 0.0) {
        auto* tk = step->GetTrack();
        G4ThreeVector dir = tk->GetMomentumDirection();
        if (dir.z() > 0.0 && G4UniformRand() < R) {
            dir.setZ(-dir.z());                 // 镜面反射:翻转 z 分量,送回结构
            tk->SetMomentumDirection(dir);
        }
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
        if (name == "structure") {
            Analysis::Instance()->AddEdep(depthUm, edepKeV, /*isFill=*/false);
            Analysis::Instance()->AddEdepMap(depthUm, xUm, edepKeV);
        } else if (name == "fill") {
            Analysis::Instance()->AddEdep(depthUm, edepKeV, /*isFill=*/true);
            Analysis::Instance()->AddEdepMap(depthUm, xUm, edepKeV);
        }
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
