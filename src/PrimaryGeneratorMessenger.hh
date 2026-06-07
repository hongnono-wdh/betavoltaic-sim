// PrimaryGeneratorMessenger.hh
// /gun/isotope 与 /ana/* 命令。
#ifndef PRIMARY_GENERATOR_MESSENGER_HH
#define PRIMARY_GENERATOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;

class PrimaryGeneratorMessenger : public G4UImessenger {
public:
    explicit PrimaryGeneratorMessenger(PrimaryGeneratorAction* pga);
    ~PrimaryGeneratorMessenger() override;
    void SetNewValue(G4UIcommand* cmd, G4String val) override;

private:
    PrimaryGeneratorAction* fPGA;
    G4UIdirectory*        fGunDir = nullptr;
    G4UIdirectory*        fAnaDir = nullptr;
    G4UIcmdWithAString*   fIsotopeCmd = nullptr;
    G4UIcmdWithAString*   fIncidenceCmd = nullptr;
    G4UIcmdWithADouble*   fEminCmd = nullptr;
    G4UIcmdWithAString*   fPrefixCmd  = nullptr;
    G4UIcmdWithAnInteger* fNTracksCmd = nullptr;
    G4UIcmdWithADouble*   fMaxDepthCmd = nullptr;
};

#endif // PRIMARY_GENERATOR_MESSENGER_HH
