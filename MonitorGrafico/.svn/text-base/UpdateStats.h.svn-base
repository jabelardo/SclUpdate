#ifndef UPDATESTATS_H
#define UPDATESTATS_H

#include <wx/hashmap.h>
#include <wx/string.h>

class UpdateStats
{
    public:
        UpdateStats();
        UpdateStats(char updateName, wxUint16 activeDownloads, wxUint16 completeDownloads);
        UpdateStats(UpdateStats const& stats);
        UpdateStats& operator=(UpdateStats const& stats);

        wxString GetUpdateName() const;
        wxString GetActiveDownloads() const;
        wxString GetCompleteDownloads() const;

    private:
        wxString updateName;
        wxString activeDownloads;
        wxString completeDownloads;
};

WX_DECLARE_HASH_MAP(char, UpdateStats, wxIntegerHash, wxIntegerEqual, UpdateStatsMap);

#endif // UPDATESTATS_H
