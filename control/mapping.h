//
// Created by james on 03/03/2020.
//

#ifndef DEEPTROUBLE_MAPPING_H
#define DEEPTROUBLE_MAPPING_H

#include <tuple>
#include <vector>
#include <optional>

static float TURN_TIME = 0.25; /// 15 minutes

using coordinates_2d_t = std::pair<float, float>;
using coordinates_3d_t = std::tuple<float, float, int>;

struct sounding_t {
    int depth;
    std::vector<float> termperatures; ///< Indexed from depth 0
};

static const float horizontal_scale = 1; ///< scale of the form (x/y) distance/horizontal unit coordinate
static const float vertical_scale = 1; ///< scale of the form depth/vertical unit coordinate

float distance_2d(std::pair<float, float>, std::pair<float, float>);
float distance_3d(std::tuple<float, float, int>, std::tuple<float, float, int>);
int bearing_2d(std::pair<float, float>, std::pair<float, float>, bool zero_centred = false);

float dot_product(const coordinates_3d_t &a, const coordinates_3d_t &b);

//std::optional<coordinates_3d_t> intercept(const coordinates_3d_t &a, const coordinates_3d_t &b, const coordinates_3d_t &c, const coordinates_3d_t &d, vector_t<coordinates_3d_t>);

#endif //DEEPTROUBLE_MAPPING_H
