// main.cc — β 能量沉积仿真引擎入口
//
// 用法:
//   betasim <run.mac>          以批处理模式执行宏文件
//   betasim                    （无参）执行内置默认宏 macros/run_default.mac
//
// 宏中通过 /det/... /gun/... /ana/... 设置参数，最后 /run/beamOn N。
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include <ctime>

int main(int argc, char** argv) {
    // 单线程，简化打分（Analysis 单例无需线程合并）
    auto* runManager =
        G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    // 随机种子：默认用时间，保证不同次运行统计独立
    G4Random::setTheEngine(new CLHEP::RanecuEngine());
    G4Random::setTheSeed(static_cast<long>(time(nullptr)));

    auto* ui = G4UImanager::GetUIpointer();

    if (argc >= 2) {
        // 批处理模式：执行指定宏
        G4String command = "/control/execute ";
        ui->ApplyCommand(command + argv[1]);
    } else {
        // 默认宏
        ui->ApplyCommand("/control/execute macros/run_default.mac");
    }

    delete runManager;
    return 0;
}
