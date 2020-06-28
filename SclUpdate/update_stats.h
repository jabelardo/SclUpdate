#ifndef SCLUPDATE_UPDATESTATS_H
#define SCLUPDATE_UPDATESTATS_H

#include <ace/SString.h>
#include <map>

namespace SclUpdate
{

class UpdateStats
{
    public:
        explicit UpdateStats(char update_name = '\0');
        UpdateStats(UpdateStats const& stats);
        UpdateStats& operator=(UpdateStats const& stats);

        ACE_UINT16 get_active_downloads() const;
        ACE_UINT16 get_complete_downloads() const;

        void increase_complete_downloads();

        void increase_active_downloads();
        void decrease_active_downloads();

    private:
        char update_name;
        ACE_UINT16 active_downloads;
        ACE_UINT16 complete_downloads;
};

typedef std::map<char, UpdateStats> UpdateStatsMap;

} // namespace SclUpdate

#endif // SCLUPDATE_UPDATESTATS_H
