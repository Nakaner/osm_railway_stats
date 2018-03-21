#ifndef OSM_RAILWAY_STATS_TYPE_COUNTS
#define OSM_RAILWAY_STATS_TYPE_COUNTS

#include <iostream>
#include <string>
#include <osmium/osm/way.hpp>

struct TypeCounts {
    std::string type;
    size_t count = 0;
    double length = 0;
    double maxspeed = 0;
    double maxspeed_no_service = 0;
    double preferred_direction = 0;
    double gauge = 0;
    double electrified_any = 0;
    double electrified = 0;
    double voltage = 0;
    double frequency = 0;
    double usage = 0;
    double service = 0;

    TypeCounts(std::string name);

    void update(const osmium::Way& way);

    std::string to_string() const;
};

template <typename TChar, typename TTraits>
inline std::basic_ostream<TChar, TTraits>& operator<<(std::basic_ostream<TChar, TTraits>& out, const TypeCounts& sums) {
    return out << sums.to_string();
}

#endif //OSM_RAILWAY_STATS_TYPE_COUNTS
