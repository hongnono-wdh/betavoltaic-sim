// EventAction.hh
// 缓存当前 event id 与是否记录轨迹，供 SteppingAction 快速查询。
#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction() = default;
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* evt) override;
    void EndOfEventAction(const G4Event* evt) override;

    G4int EventID() const { return fEventID; }
    bool  RecordTrack() const { return fRecordTrack; }

    // 每事件只在 β 首次越过结构顶面时累加一次"入射能量"
    bool  IncidentCounted() const { return fIncidentCounted; }
    void  SetIncidentCounted() { fIncidentCounted = true; }
    bool  BackCounted() const { return fBackCounted; }
    void  SetBackCounted() { fBackCounted = true; }

private:
    G4int fEventID = 0;
    bool  fRecordTrack = false;
    bool  fIncidentCounted = false;
    bool  fBackCounted = false;
};

#endif // EVENT_ACTION_HH
