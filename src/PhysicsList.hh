// PhysicsList.hh
// Livermore 低能电磁物理（与论文一致），并设置纳米尺度的低产生阈。
#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    ~PhysicsList() override = default;
    void SetCuts() override;
};

#endif // PHYSICS_LIST_HH
