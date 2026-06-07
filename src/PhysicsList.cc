// PhysicsList.cc
#include "PhysicsList.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProductionCutsTable.hh"

PhysicsList::PhysicsList() {
    // 论文同款：Livermore 低能电磁模型
    RegisterPhysics(new G4EmLivermorePhysics());

    // 低能下限，分辨纳米尺度沉积
    auto* emp = G4EmParameters::Instance();
    emp->SetMinEnergy(100 * eV);
    emp->SetMaxEnergy(1 * MeV);
    emp->SetLowestElectronEnergy(50 * eV);
    emp->SetFluo(true);
    emp->SetAuger(true);

    // 产生阈下限放到 10 nm 量级
    G4ProductionCutsTable::GetProductionCutsTable()
        ->SetEnergyRange(10 * eV, 1 * MeV);
}

void PhysicsList::SetCuts() {
    // 纳米结构：用很小的 range cut（10 nm）
    SetDefaultCutValue(10 * nm);
    SetCutsWithDefault();
}
