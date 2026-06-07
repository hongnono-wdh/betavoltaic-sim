// Analysis.cc
#include "Analysis.hh"

#include "G4SystemOfUnits.hh"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cmath>

Analysis* Analysis::Instance() {
    static Analysis inst;
    return &inst;
}

void Analysis::Configure(const G4String& prefix, G4int nTracks, G4double maxDepthUm) {
    fPrefix = prefix;
    fNTracks = nTracks;
    fMaxDepthUm = maxDepthUm;
    fBinWidthUm = 0.05;
    fNBins = static_cast<int>(std::lround(fMaxDepthUm / fBinWidthUm));
    Reset();
}

void Analysis::Reset() {
    fEddWall.assign(fNBins, 0.0);
    fEddFill.assign(fNBins, 0.0);
    fTotalIncidentKeV = 0.0;
    fNIncident = 0;
    fNBackscatter = 0;
    fHasFill = false;
    fTrackPoints.clear();
    fEdepPoints.clear();
}

int Analysis::bin(G4double depthUm) const {
    if (depthUm < 0.0) return -1;
    int b = static_cast<int>(depthUm / fBinWidthUm);
    if (b < 0 || b >= fNBins) return -1;
    return b;
}

void Analysis::AddEdep(G4double depthUm, G4double edepKeV, bool isFill) {
    int b = bin(depthUm);
    if (b < 0) return;
    if (isFill) { fEddFill[b] += edepKeV; fHasFill = true; }
    else        { fEddWall[b] += edepKeV; }
}

void Analysis::AddTrackPoint(G4int eventID, G4double xUm, G4double zUm) {
    if (eventID >= fNTracks) return;
    fTrackPoints.push_back({static_cast<double>(eventID), xUm, zUm});
}

void Analysis::AddEdepPoint(G4int eventID, G4double xUm, G4double zUm, G4double edepKeV) {
    if (eventID >= fNTracks) return;
    fEdepPoints.push_back({static_cast<double>(eventID), xUm, zUm, edepKeV});
}

void Analysis::Write() {
    const double denom = (fTotalIncidentKeV > 0.0) ? fTotalIncidentKeV : 1.0;

    // —— EDD/EDR CSV —— //
    {
        std::ofstream f(fPrefix + "_edd_edr.csv");
        f << "depth_um,edd_wall,edd_fill,edd_total,edr_wall,edr_fill,edr_total\n";
        double cumW = 0.0, cumF = 0.0, cumT = 0.0;
        f << std::setprecision(8);
        for (int b = 0; b < fNBins; ++b) {
            const double depth = (b + 0.5) * fBinWidthUm;
            const double ew = fEddWall[b], ef = fEddFill[b], et = ew + ef;
            cumW += ew; cumF += ef; cumT += et;
            // EDD 归一化为 (沉积能量分数 / μm)
            const double eddW = ew / denom / fBinWidthUm;
            const double eddF = ef / denom / fBinWidthUm;
            const double eddT = et / denom / fBinWidthUm;
            f << depth << ',' << eddW << ',' << eddF << ',' << eddT << ','
              << (cumW/denom) << ',' << (cumF/denom) << ',' << (cumT/denom) << '\n';
        }
    }

    // —— 轨迹 CSV —— //
    {
        std::ofstream f(fPrefix + "_tracks.csv");
        f << "event_id,x_um,z_um\n" << std::setprecision(6);
        for (auto& p : fTrackPoints)
            f << static_cast<int>(p[0]) << ',' << p[1] << ',' << p[2] << '\n';
    }

    // —— 沉积点 CSV —— //
    {
        std::ofstream f(fPrefix + "_edep_points.csv");
        f << "event_id,x_um,z_um,edep_kev\n" << std::setprecision(6);
        for (auto& p : fEdepPoints)
            f << static_cast<int>(p[0]) << ',' << p[1] << ',' << p[2] << ',' << p[3] << '\n';
    }

    // —— 自检摘要 —— //
    auto edrAt = [&](double dUm, const std::vector<double>& h)->double {
        double cum = 0.0; int bmax = bin(dUm);
        if (bmax < 0) bmax = fNBins - 1;
        for (int b = 0; b <= bmax && b < fNBins; ++b) cum += h[b];
        return cum / denom * 100.0;
    };
    // 总谱峰值深度
    int peakBin = 0; double peakVal = -1;
    for (int b = 0; b < fNBins; ++b) {
        double v = fEddWall[b] + fEddFill[b];
        if (v > peakVal) { peakVal = v; peakBin = b; }
    }

    auto edrTotAt = [&](double dUm)->double {
        double cum = 0.0; int bmax = bin(dUm); if (bmax < 0) bmax = fNBins - 1;
        for (int b = 0; b <= bmax && b < fNBins; ++b) cum += fEddWall[b] + fEddFill[b];
        return cum / denom * 100.0;
    };

    std::cout << "\n===== Analysis self-check [" << fPrefix << "] =====\n";
    std::cout << "Total incident KE      : " << fTotalIncidentKeV << " keV\n";
    std::cout << "EDD peak depth (total) : " << (peakBin + 0.5) * fBinWidthUm << " um\n";
    std::cout << "EDR(total) @2um  : " << edrTotAt(2.0)  << "%\n";
    std::cout << "EDR(total) @9um  : " << edrTotAt(9.0)  << "%\n";
    std::cout << "EDR(total) @10um : " << edrTotAt(10.0) << "%\n";
    if (fHasFill)
        std::cout << "EDR(fill)  @5um  : " << edrAt(5.0, fEddFill) << "%\n";
    if (fNIncident > 0) {
        double eta = static_cast<double>(fNBackscatter) / fNIncident;
        std::cout << "Backscatter eta (number) : " << eta
                  << "  (" << fNBackscatter << "/" << fNIncident << ")\n";
    }
    std::cout << "Wrote: " << fPrefix << "_edd_edr.csv / _tracks.csv / _edep_points.csv\n";
    std::cout << "=================================================\n";
}
