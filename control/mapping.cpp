//
// Created by james on 03/03/2020.
//

#include "mapping.h"

#include <cmath>

using namespace std;

float distance_2d(std::pair<float, float> a, std::pair<float, float> b) {
    return horizontal_scale*sqrt((a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second));
}

float distance_3d(std::tuple<float, float, int> a, std::tuple<float, float, int> b) {

    float distance = distance_2d({get<0>(a), get<1>(a)}, {get<0>(b), get<1>(b)});

    auto depth_diff = vertical_scale*(float) (get<2>(a)-get<2>(b));

    return sqrt(distance*distance + depth_diff*depth_diff);
}

/**
 *
 * @param observer coordinates
 * @param object coordinates
 * @param zero_centred center bearing on 0, between -180 and 180, otherwise 0 and 360
 * @return bearing (from north, clockwise) from observer to object
 */
int bearing_2d(std::pair<float, float> observer, std::pair<float, float> object, bool zero_centred) {
    auto x = object.first - observer.first;
    auto y = object.second - observer.second;
    auto angle = 180*atan(x / y)/M_PI;
    if (y < 0)
        angle += 180;

    if (zero_centred){
        if (angle < 0){
            angle += 360;
        }
    }

    return angle;
}

float dot_product(const coordinates_3d_t &a, const coordinates_3d_t &b) {
    auto dot = get<0>(a) * get<0>(b);
    dot += get<1>(a) * get<1>(b);
    dot += (float) get<2>(a) * get<2>(b);
    return  dot;
}
/*
std::optional<coordinates_3d_t>
intercept(const coordinates_3d_t &a, const coordinates_3d_t &b, const coordinates_3d_t &c, const coordinates_3d_t &d,
          vector_t<coordinates_3d_t>) {

    return coordinates_3d_t();
}

 */