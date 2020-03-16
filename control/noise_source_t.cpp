//
// Created by james on 03/03/2020.
//

#include <cmath>
#include "noise_source_t.h"
#include "mapping.h"
#include "file_handle_t.h"

using namespace std;

noise_source_t::noise_report_t noise_source_t::get_report(const std::pair<float, float> &rx_coordinates) const {

    noise_report_t report;

    if (NOISE_MODEL == SIMPLE_STEP) {
        auto distance = distance_2d({x, y}, rx_coordinates);
        if (distance < SHORT_RANGE) {
            // Sound within short range
            report.magnitude = type == SUBMARINE ? submarine->get_sound().magnitude : magnitude;

        } else if (distance < LONG_RANGE) {
            // Sound within long range
            report.magnitude = magnitude / 2;
        } else {
            // Sound indistinguishable from background noise
            report.magnitude = 0;
        }

        // TODO Thermocline check and cutoff
    } else if (NOISE_MODEL == INV_SQR){

        auto adjusted_magnitude = magnitude;
        // TODO Thermocline check and cutoff
        if (false /* TODO */) {
            adjusted_magnitude -= 3; // Halving
        }
        auto distance = distance_2d({x, y}, rx_coordinates);
        report.magnitude = adjusted_magnitude - 20*log10(distance);
    }

    if (report.magnitude < MAGNITUDE_CUTOFF)
        return report;

    switch (type) {
        case SUBMARINE:
            report.description = "Suspected submarine contact:" + submarine->get_sound().description;
            break;

        case SURFACE:
            report.description = "Suspected surface contact";
            break;

        case EXPLOSION:
            report.description = "Underwater explosion";
            break;

        case MARINE_LIFE:
            report.description = "Whale sounds";
            break;

        case GEOLOGICAL:
            report.description = "Anomalous geological sound";
            break;
        case COLLISION:
            report.description = "Sound of rent steel, suspected collision";
            break;
        case UNIDENTIFIED:
            report.description = "Er what? Sir come and have a listen to this!! No, no clue either";
            break;
    }
    

    report.type = type;
    report.direction = bearing_2d(rx_coordinates, {x, y});

    if (type == SUBMARINE) {
        report.speed = submarine->get_speed();
        report.heading = submarine->getDirection();
    }

    return report;
}

noise_source_t::noise_source_t(submarine_t& source_submarine) : submarine(&source_submarine){
    type = SUBMARINE;
    x = source_submarine.getLocx();
    y = source_submarine.getLocy();
}

noise_source_t::noise_source_t(const std::string &file_path) {
    auto handle = file_handle_t(file_path, file_handle_t::io_t::IN);
    auto type_str = handle.parse_string_line();

    if (type_str == "Geological")
        type = noise_type_t::GEOLOGICAL;
    else if (type_str == "Biological")
        type = noise_type_t::MARINE_LIFE;
    else if (type_str == "Unidentified")
        type = noise_type_t::UNIDENTIFIED;

    // TODO and so on

    x = handle.parse_int_line();
    y = handle.parse_int_line();
    description = handle.parse_string_line();
    magnitude = handle.parse_int_line();
}
