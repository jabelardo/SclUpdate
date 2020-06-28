#include "update_stats.h"

namespace SclUpdate
{

UpdateStats::UpdateStats(char update_name)
    : update_name(update_name)
    , active_downloads(0)
    , complete_downloads(0)
{
}

UpdateStats::UpdateStats(UpdateStats const& stats)
    : update_name(stats.update_name)
    , active_downloads(stats.active_downloads)
    , complete_downloads(stats.complete_downloads)
{
}

UpdateStats&
UpdateStats::operator=(UpdateStats const& stats)
{
    if (this != &stats) {
        update_name = stats.update_name;
        active_downloads = stats.active_downloads;
        complete_downloads = stats.complete_downloads;
    }
    return *this;
}

ACE_UINT16
UpdateStats::get_active_downloads() const
{
    return active_downloads;
}

ACE_UINT16
UpdateStats::get_complete_downloads() const
{
    return complete_downloads;
}

void
UpdateStats::increase_complete_downloads()
{
    ++complete_downloads;
}

void
UpdateStats::increase_active_downloads()
{
    ++active_downloads;
}

void
UpdateStats::decrease_active_downloads()
{
    --active_downloads;
}

} // namespace SclUpdate
