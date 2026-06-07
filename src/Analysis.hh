// Analysis.hh
// 单例：聚合 EDD/EDR 深度直方图（区分管壁/电解液）+ 轨迹与沉积点，写出 CSV。
// 单线程运行，无需线程合并。
#ifndef ANALYSIS_HH
#define ANALYSIS_HH

#include "globals.hh"
#include <vector>
#include <string>
#include <array>

class Analysis {
public:
    static Analysis* Instance();

    void Configure(const G4String& prefix, G4int nTracks, G4double maxDepthUm);
    void Reset();

    // 累加一次能量沉积：depthUm = -z；edepKeV；isFill=true 表示在电解液内。
    void AddEdep(G4double depthUm, G4double edepKeV, bool isFill);

    // 源发射的入射动能（归一化 EDR 的分母）。
    void AddIncidentEnergy(G4double keV) { fTotalIncidentKeV += keV; }

    // 数量背散射系数 η 计数(校准用):进入结构的 primary 数 / 其中背散射出顶面的数。
    void AddIncidentCount()    { ++fNIncident; }
    void AddBackscatterCount() { ++fNBackscatter; }

    // 轨迹记录（仅前 nTracks 个事件）。
    bool ShouldRecordTrack(G4int eventID) const { return eventID < fNTracks; }
    void AddTrackPoint(G4int eventID, G4double xUm, G4double zUm);
    void AddEdepPoint(G4int eventID, G4double xUm, G4double zUm, G4double edepKeV);

    // 运行结束写出所有 CSV，并打印自检摘要。
    void Write();

    G4double TotalIncidentKeV() const { return fTotalIncidentKeV; }

    // 顶面再俘获反射率 R(0=无,1=完美);β 向上逃逸时以 R 概率被反射回结构
    void     SetReflectR(G4double r) { fReflectR = r; }
    G4double ReflectR() const { return fReflectR; }

private:
    Analysis() = default;
    int bin(G4double depthUm) const;

    G4String fPrefix = "run";
    G4int    fNTracks = 200;
    G4double fMaxDepthUm = 15.0;
    G4int    fNBins = 300;          // 50 nm/bin
    G4double fBinWidthUm = 0.05;

    std::vector<double> fEddWall;   // 每箱沉积能量（管壁/实心），keV
    std::vector<double> fEddFill;   // 每箱沉积能量（电解液），keV
    double fTotalIncidentKeV = 0.0;
    double fReflectR = 0.0;       // 顶面再俘获反射率(config;不随 Reset 清零)
    long   fNIncident = 0;        // 进入结构的 primary 计数(η 分母)
    long   fNBackscatter = 0;     // 背散射出顶面的 primary 计数(η 分子)
    bool   fHasFill = false;

    // 轨迹：event_id, x, z
    std::vector<std::array<double,3>> fTrackPoints;
    // 沉积点：event_id, x, z, edep
    std::vector<std::array<double,4>> fEdepPoints;
};

#endif // ANALYSIS_HH
