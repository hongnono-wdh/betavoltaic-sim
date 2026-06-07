// PrimaryGeneratorMessenger.cc
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* pga)
    : fPGA(pga) {
    fGunDir = new G4UIdirectory("/gun/");
    fGunDir->SetGuidance("β 源设置");
    fAnaDir = new G4UIdirectory("/ana/");
    fAnaDir->SetGuidance("分析/输出设置");

    fIsotopeCmd = new G4UIcmdWithAString("/gun/isotope", this);
    fIsotopeCmd->SetGuidance("同位素: Ni-63 | H-3 | C-14 | Pm-147");
    fIsotopeCmd->SetParameterName("iso", false);
    fIsotopeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fIncidenceCmd = new G4UIcmdWithAString("/gun/incidence", this);
    fIncidenceCmd->SetGuidance("入射角模型: isotropic(默认,真实4π) | hemisphere(2π下行) | normal(法向)");
    fIncidenceCmd->SetParameterName("mode", false);
    fIncidenceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fPrefixCmd = new G4UIcmdWithAString("/ana/prefix", this);
    fPrefixCmd->SetGuidance("输出 CSV 文件名前缀");
    fPrefixCmd->SetParameterName("prefix", false);
    fPrefixCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fNTracksCmd = new G4UIcmdWithAnInteger("/ana/nTracks", this);
    fNTracksCmd->SetGuidance("记录轨迹的事件数");
    fNTracksCmd->SetParameterName("n", false);
    fNTracksCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaxDepthCmd = new G4UIcmdWithADouble("/ana/maxDepthUm", this);
    fMaxDepthCmd->SetGuidance("深度直方图最大深度 (um)");
    fMaxDepthCmd->SetParameterName("d", false);
    fMaxDepthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
    delete fIsotopeCmd; delete fIncidenceCmd; delete fPrefixCmd; delete fNTracksCmd; delete fMaxDepthCmd;
    delete fGunDir; delete fAnaDir;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if      (cmd == fIsotopeCmd)   fPGA->SetIsotope(val);
    else if (cmd == fIncidenceCmd) fPGA->SetIncidence(val);
    else if (cmd == fPrefixCmd)    fPGA->SetAnaPrefix(val);
    else if (cmd == fNTracksCmd)  fPGA->SetAnaNTracks(fNTracksCmd->GetNewIntValue(val));
    else if (cmd == fMaxDepthCmd) fPGA->SetAnaMaxDepthUm(fMaxDepthCmd->GetNewDoubleValue(val));
}
