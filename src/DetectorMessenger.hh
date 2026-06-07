// DetectorMessenger.hh
// 暴露 /det/... 宏命令，供 run.mac 在运行前设置几何参数。
#ifndef DETECTOR_MESSENGER_HH
#define DETECTOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class DetectorMessenger : public G4UImessenger {
public:
    explicit DetectorMessenger(DetectorConstruction* det);
    ~DetectorMessenger() override;
    void SetNewValue(G4UIcommand* cmd, G4String val) override;

private:
    DetectorConstruction* fDet;
    G4UIdirectory*        fDir = nullptr;
    G4UIcmdWithAString*   fStructureCmd = nullptr;
    G4UIcmdWithAString*   fMaterialCmd  = nullptr;
    G4UIcmdWithAString*   fFillCmd      = nullptr;
    G4UIcmdWithAString*   fSubstrateCmd = nullptr;
    G4UIcmdWithADouble*   fHeightCmd    = nullptr;
    G4UIcmdWithADouble*   fTubeOuterCmd = nullptr;
    G4UIcmdWithADouble*   fTubeInnerCmd = nullptr;
    G4UIcmdWithADouble*   fPitchCmd     = nullptr;
    G4UIcmdWithAnInteger* fArrayNCmd    = nullptr;
};

#endif // DETECTOR_MESSENGER_HH
