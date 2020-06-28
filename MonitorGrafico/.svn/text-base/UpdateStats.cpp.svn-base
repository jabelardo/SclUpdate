#include "UpdateStats.h"

UpdateStats::UpdateStats()
    : updateName()
    , activeDownloads()
    , completeDownloads()
{
}

UpdateStats::UpdateStats(char updateName, wxUint16 activeDownloads, wxUint16 completeDownloads)
    : updateName(wxChar(updateName))
    , activeDownloads(wxString::Format(_T("%d"), activeDownloads))
    , completeDownloads(wxString::Format(_T("%d"), completeDownloads))
{
}

UpdateStats::UpdateStats(UpdateStats const& stats)
    : updateName(stats.updateName)
    , activeDownloads(stats.activeDownloads)
    , completeDownloads(stats.completeDownloads)
{
}

UpdateStats&
UpdateStats::operator=(UpdateStats const& stats)
{
    if (this != &stats) {
        updateName = stats.updateName;
        activeDownloads = stats.activeDownloads;
        completeDownloads = stats.completeDownloads;
    }
    return *this;
}

wxString
UpdateStats::GetUpdateName() const
{
    return updateName;
}

wxString
UpdateStats::GetActiveDownloads() const
{
    return activeDownloads;
}

wxString
UpdateStats::GetCompleteDownloads() const
{
    return completeDownloads;
}
