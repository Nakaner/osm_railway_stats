#include "type_counts.hpp"
#include <osmium/geom/haversine.hpp>
#include <sstream>

TypeCounts::TypeCounts(std::string name) :
    type(name) {}

void TypeCounts::update(const osmium::Way& way) {
    const char* railway = way.get_value_by_key("railway");
    if (!railway || strcmp(railway, type.c_str())) {
       return;
    }
    double l = osmium::geom::haversine::distance(way.nodes());
    length += l;
    ++count;
    maxspeed += way.tags().has_key("maxspeed") * l;
    maxspeed_no_service += (way.tags().has_key("maxspeed") && !way.tags().has_key("service")) * l;
    preferred_direction += way.tags().has_key("railway:preferred_direction") * l;
    gauge += way.tags().has_key("gauge") * l;
    const char* electrified_val = way.get_value_by_key("electrified");
    if (electrified_val) {
        electrified_any += l;
        if (!strcmp(electrified_val, "contact_line") || !strcmp(electrified_val, "rail")) {
            electrified += l;
        }
    }
    voltage += way.tags().has_key("voltage") * l;
    frequency += way.tags().has_key("frequency") * l;
    const char* usage_val = way.get_value_by_key("usage");
    const char* service_val = way.get_value_by_key("service");
    if (usage_val && !service_val && (!strcmp(usage_val, "main") || !strcmp(usage_val, "branch")
            || !strcmp(usage_val, "military") || !strcmp(usage_val, "test") || !strcmp(usage_val, "industrial"))) {
        usage += l;
    } else if (service_val && !usage_val && (!strcmp(service_val, "siding") || !strcmp(service_val, "yard")
            || !strcmp(service_val, "crossover") || !strcmp(service_val, "spur"))) {
        service += l;
    } else if (usage_val && (!strcmp(usage_val, "industrial") || !strcmp(usage_val, "military")) && service_val
            && (!strcmp(service_val, "siding") || !strcmp(service_val, "yard")
            || !strcmp(service_val, "crossover") || !strcmp(service_val, "spur"))) {
        usage += l;
        service += l;
    }
}

std::string TypeCounts::to_string() const {
    std::ostringstream str;
    str.setf(std::ios::fixed, std::ios::floatfield);
    str.precision(1);
    str << "Type: " << type;
    str << "\n  total count: ";
    str << count;
    str << "\n  total length: ";
    str << length/1000;
    str << "\n  with maxspeed: ";
    str << maxspeed/1000;
    str << "\n  with maxspeed (no service tracks): ";
    str << maxspeed_no_service/1000;
    str << "\n  with railway:preferred_direction: ";
    str << preferred_direction/1000;
    str << "\n  with gauge: ";
    str << gauge/1000;
    str << "\n  with electrified: ";
    str << electrified_any/1000;
    str << "\n  electrified tracks: ";
    str << electrified/1000;
    str << "\n  with voltage: ";
    str << voltage/1000;
    str << "\n  with frequency: ";
    str << frequency/1000;
    str << "\n  with valid usage: ";
    str << usage/1000;
    str << "\n  with valid service: ";
    str << service/1000;
    str << '\n';
    return str.str();
}
