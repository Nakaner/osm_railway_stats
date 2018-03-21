
// The code in this file is released into the Public Domain.

#include <iostream>

#include <osmium/io/any_input.hpp>
#include <osmium/geom/haversine.hpp>
#include <osmium/visitor.hpp>

#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>

#include "type_counts.hpp"

typedef osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location> index_type;
typedef osmium::handler::NodeLocationsForWays<index_type> location_handler_type;

struct RailHandler : public osmium::handler::Handler {

    TypeCounts sums_rail;
    TypeCounts sums_light_rail;
    TypeCounts sums_tram;
    TypeCounts sums_subway;
    TypeCounts sums_narrow_gauge;

    int main_signals = 0;
    int combined_signals = 0;
    int distant_signals = 0;
    int switches = 0;
    int switches_nr = 0;

    RailHandler() :
        sums_rail("rail"),
        sums_light_rail("light_rail"),
        sums_tram("tram"),
        sums_subway("subway"),
        sums_narrow_gauge("narrow_gauge") {}

    void node(const osmium::Node& node) {
        const char* railway = node.get_value_by_key("railway", "");
        const char* signal_main = node.get_value_by_key("railway:signal:main", "");
        const char* signal_distant = node.get_value_by_key("railway:signal:distant", "");
        const char* signal_combined = node.get_value_by_key("railway:signal:combined", "");
        if (!strcmp(railway, "switch")) {
            switches++;
            if (node.get_value_by_key("ref")) {
                switches_nr++;
            }
        }

        if (strcmp(railway, "signal")) {
            return;
        }
        if (!strcmp(signal_main, "DE-ESO:hp") || !strcmp(signal_main, "DE-ESO:ks") || !strcmp(signal_main, "DE-ESO:hl") || !strcmp(signal_main, "DE-ESO:sk") || !strcmp(signal_main, "DE-ESO:sv") ) {
            if (!strcmp(signal_distant, "DE-ESO:ks") || !strcmp(signal_distant, "DE-ESO:hl")) {
                return;
            }
            main_signals++;
        } else if (!strcmp(signal_combined, "DE-ESO:ks") || !strcmp(signal_combined, "DE-ESO:hl") || !strcmp(signal_combined, "DE-ESO:sv") || !strcmp(signal_combined, "DE-ESO:sk") ) {
            combined_signals++;
        }
        if (!strcmp(signal_distant, "DE-ESO:vr") || !strcmp(signal_distant, "DE-ESO:ks") || !strcmp(signal_distant, "DE-ESO:hl") || !strcmp(signal_distant, "DE-ESO:sk")) {
            distant_signals++;
        }
    }

    void way(const osmium::Way& way) {
        sums_rail.update(way);
        sums_light_rail.update(way);
        sums_tram.update(way);
        sums_subway.update(way);
        sums_narrow_gauge.update(way);
    }

};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
        exit(1);
    }

    osmium::io::Reader reader(argv[1], osmium::osm_entity_bits::node | osmium::osm_entity_bits::way);

    index_type index;
    location_handler_type location_handler(index);

    RailHandler handler;

    osmium::apply(reader, location_handler, handler);
    std::cout << handler.sums_rail;
    std::cout << handler.sums_light_rail;
    std::cout << handler.sums_tram;
    std::cout << handler.sums_subway;
    std::cout << handler.sums_narrow_gauge;
    std::cout << "length main signals: " << handler.main_signals << std::endl;
    std::cout << "length combined signal: " << handler.combined_signals << std::endl;
    std::cout << "length distant signals: " << handler.distant_signals << std::endl;
    std::cout << "length switches: " << handler.switches  <<  std::endl;
    std::cout << "length switches with ref: " << handler.switches_nr <<  std::endl;
}

