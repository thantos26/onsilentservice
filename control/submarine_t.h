//
// Created by james on 02/03/2020.
//

#ifndef DEEPTROUBLE_SUBMARINE_T_H
#define DEEPTROUBLE_SUBMARINE_T_H

#include <string>
#include <utility>
#include <memory>

#include "mapping.h"


struct sound_t {
    std::string description;
    int magnitude;
};

struct speed_setting_t {
    std::string order;
    int noise;
    int power;
    int distance;

    [[nodiscard]] static speed_setting_t parse_from_line(const std::string &line);
};

struct power_threshold_t {
    int generated;
    int noise;
    [[nodiscard]] static power_threshold_t parse_from_line(const std::string &line);
};

class noise_source_t;

class submarine_t {
    int id;
    std::string name, boat_class;
    float locx = 0, locy = 0;
    int direction = 0;
    int depth = 0;

    coordinates_3d_t last_position;

    sound_t sound;

    // Class data
    int emergency_turn;
    int max_depth;
    int max_turn;
    int max_bubble;
    int ballast;
    int power_shift;
    int power_output;
    int compressed_air;
    std::vector<speed_setting_t> speeds;
    std::vector<power_threshold_t> power_thresholds;

    int get_flooded_compartments();
    void set_class_data(const std::string & file_path);

public:
    std::shared_ptr<noise_source_t> noise;

    submarine_t() = default;
    explicit submarine_t(const std::string& file_path);
    std::pair<float, float> move(float distance, int direction, int bubble, int power);

    bool export_sub(const std::string & file_path) const;

    [[nodiscard]] float getLocx() const;
    [[nodiscard]] float getLocy() const;
    [[nodiscard]] coordinates_3d_t getLastPosition() const;
    [[nodiscard]] int getId() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getBoatClass() const;
    [[nodiscard]] int getDirection() const;
    [[nodiscard]] int getDepth() const;

    [[nodiscard]] sound_t get_sound() const;

    [[nodiscard]] float get_speed() const;

};


#endif //DEEPTROUBLE_SUBMARINE_T_H
