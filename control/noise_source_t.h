//
// Created by james on 03/03/2020.
//

#ifndef DEEPTROUBLE_NOISE_SOURCE_T_H
#define DEEPTROUBLE_NOISE_SOURCE_T_H

#include <utility>
#include <string>
#include "submarine_t.h"

/// Static definition that selects the underwater acoustic model
const enum {
    SIMPLE_STEP, ///< Noises are either loud (within short range), quiet (long range) or silent (beyond that)
    INV_SQR
} NOISE_MODEL = INV_SQR;

static const double SHORT_RANGE = 500;
static const double LONG_RANGE = 1000;

/**
 * Structure models a noise source.
 */
struct noise_source_t {

    static const int MAGNITUDE_CUTOFF = -20;

    enum noise_type_t{
        SUBMARINE = 0,
        SURFACE = 1,
        MARINE_LIFE = 2,
        GEOLOGICAL = 3,
        EXPLOSION = 4,
        COLLISION = 5,
        UNIDENTIFIED = 6
    };

    struct noise_report_t {
        noise_type_t type;
        std::string description;
        float magnitude;
        int heading;
        float speed = 0;
        int direction = 0;
    };

    float x, y;
    int depth;
    noise_type_t type;
    float magnitude = 0;
    std::string description;
    const submarine_t* submarine = nullptr;

    noise_source_t() = default;
    explicit noise_source_t(submarine_t& submarine);
    explicit noise_source_t(const std::string& file_path);

    [[nodiscard]] noise_report_t get_report(const std::pair<float, float> &rx_coordinates) const;
};


#endif //DEEPTROUBLE_NOISE_SOURCE_T_H
