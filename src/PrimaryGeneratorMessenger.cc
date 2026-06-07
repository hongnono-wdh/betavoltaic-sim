// PrimaryGeneratorMessenger.cc
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

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

    fEminCmd = new G4UIcmdWithADouble("/gun/eMinKeV", this);
    fEminCmd->SetGuidance("有效源能量下限(keV);>0 丢弃更软的 β,模拟源自吸收硬化");
    fEminCmd->SetParameterName("emin", false);
    fEminCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSourceModeCmd = new G4UIcmdWithAString("/gun/sourceMode", this);
    fSourceModeCmd->SetGuidance("源模式: surface(贴顶面) | volume(Ni 源体内,含自吸收)");
    fSourceModeCmd->SetParameterName("mode", false);
    fSourceModeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMonoCmd = new G4UIcmdWithADouble("/gun/monoKeV", this);
    fMonoCmd->SetGuidance("单能电子能量 (keV);>0 用单能(校准 η/射程),0 用 β 谱");
    fMonoCmd->SetParameterName("e", false);
    fMonoCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

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

    fReflectCmd = new G4UIcmdWithADouble("/ana/reflectR", this);
    fReflectCmd->SetGuidance("顶面再俘获反射率 R (0..1);β 向上逃逸时按此概率反射回结构");
    fReflectCmd->SetParameterName("R", false);
    fReflectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
    delete fIsotopeCmd; delete fIncidenceCmd; delete fEminCmd; delete fSourceModeCmd; delete fMonoCmd; delete fPrefixCmd; delete fNTracksCmd; delete fMaxDepthCmd; delete fReflectCmd;
    delete fGunDir; delete fAnaDir;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if      (cmd == fIsotopeCmd)   fPGA->SetIsotope(val);
    else if (cmd == fIncidenceCmd) fPGA->SetIncidence(val);
    else if (cmd == fEminCmd)      fPGA->SetEminKeV(fEminCmd->GetNewDoubleValue(val));
    else if (cmd == fSourceModeCmd) fPGA->SetSourceMode(val);
    else if (cmd == fMonoCmd)      fPGA->SetMonoKeV(fMonoCmd->GetNewDoubleValue(val));
    else if (cmd == fPrefixCmd)    fPGA->SetAnaPrefix(val);
    else if (cmd == fNTracksCmd)  fPGA->SetAnaNTracks(fNTracksCmd->GetNewIntValue(val));
    else if (cmd == fMaxDepthCmd) fPGA->SetAnaMaxDepthUm(fMaxDepthCmd->GetNewDoubleValue(val));
    else if (cmd == fReflectCmd)  Analysis::Instance()->SetReflectR(fReflectCmd->GetNewDoubleValue(val));
}
