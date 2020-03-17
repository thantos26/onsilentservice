//
// Created by james on 02/03/2020.
//

#include <cmath>
#include <iostream>
#include "submarine_t.h"
#include "terminal.h"
#include "file_handle_t.h"
#include "noise_source_t.h"

using namespace std;

void submarine_t::set_class_data(const string &file_path) {
    auto handle = file_handle_t(file_path, file_handle_t::io_t::IN);
    string input_buffer = handle.parse_string_line();
    ballast = handle.parse_int_line();
    compressed_air = handle.parse_int_line();
    power_shift = handle.parse_int_line();
    max_turn = handle.parse_int_line();
    emergency_turn = handle.parse_int_line();
    max_depth = handle.parse_int_line();
    max_bubble = handle.parse_int_line();

    auto speed_list = handle.parse_indented_list();
    for (const auto &speed_line : speed_list.second) {
        speeds.push_back(speed_setting_t::parse_from_line(speed_line));
    }

    auto power_list = handle.parse_indented_list();
    for (const auto &power_line : power_list.second) {
        power_thresholds.push_back(power_threshold_t::parse_from_line(power_line));
    }

}

submarine_t::submarine_t(const string &filepath) {
    auto handle = file_handle_t(filepath, file_handle_t::io_t::IN);

    id = handle.parse_int_line();
    name = handle.parse_string_line();
    boat_class = handle.parse_string_line();
    locx = handle.parse_int_line();
    locy = handle.parse_int_line();
    depth = handle.parse_int_line();
    direction = handle.parse_int_line();

    set_class_data("../boat_classes/" + boat_class + "_class");

}

bool submarine_t::export_sub(const string &file_path) const {
    file_handle_t handle(file_path, file_handle_t::io_t::OUT);
    if (handle.out_handle) {
        handle.out_handle << "ID: " << id << endl;
        handle.out_handle << "Name: " << name << endl;
        handle.out_handle << "Class: " << boat_class << endl;
        handle.out_handle << "location (x): " << locx << endl;
        handle.out_handle << "location (y): " << locy << endl;
        handle.out_handle << "depth: " << depth << endl;
        handle.out_handle << "facing: " << direction << endl; // As bearing
        handle.out_handle << "power output: " << power_output << endl;
        handle.out_handle << "compressed air reserve: " << compressed_air << endl;
        return true;
    }
    return false;
}

std::pair<float, float> submarine_t::move(float distance, int direction, int bubble, int power) {

    last_position = {locx, locy, depth};

    // Checks the bounds of input data:

    // Check bounds of power
    while (get_power_setting(power).generated < power) {
        cerr << "Submarine cannot produce " << power << " power, maximum is " << power_thresholds.back().generated
             << endl;
        if (terminal::query("Are you sure? ") == "y")
            break;
        power = stoi(terminal::query("New power: "));
    }

    // Bounds check for course
    while (abs(direction) > max_turn) {
        cerr << "Submarine " << id << "-" << name << " ordered to move: " << to_string(direction) <<
             "°, can only make " << to_string(max_turn) << "°" << endl;

        if (terminal::query("Are you sure? ") == "y")
            break;
        direction = stoi(terminal::query("New bearing: "));
    }

    // Bounds check for course speed
    while (distance > speeds.back().distance) {

        cerr << "Submarine " << id << "-" << name << " ordered to move: " << to_string((int) distance) <<
             "m, can only make " << to_string(speeds.back().distance) << 'm' << endl;

        if (terminal::query("Are you sure? ") == "y")
            break;
        distance = stoi(terminal::query("New distance: "));


    }

    // Check speeds against power bounds
    speed_setting_t speed_setting = get_speed_setting(distance);
    while (speed_setting.power > power) {
        cerr << "Submarine " << id << "-" << name << " ordered to move: " << to_string((int) distance)
             << "m, impossible under " << power << " power" << endl;
        if (terminal::query("Are you sure? ") == "y")
            break;
        distance = stoi(terminal::query("New distance: "));
        speed_setting = get_speed_setting(distance);
    }


    // Make changes
    depth += (get_flooded_compartments() - bubble);

    if (depth < 0) {
        depth = 0;
    }

    this->direction = direction;

    speed_setting_t speed = speeds[0];
    for (int i = 1; i < speeds.size(); i++) {
        if (distance < speeds[i].distance) {
            break;
        }
        speed = speeds[i];
    }

    sound.magnitude = speed.noise; // TODO add in reactor powering

    auto course_rad = M_PI * ((float) direction) / 180.0;
    auto locx_n = locx + distance * sin(course_rad) / horizontal_scale;
    auto locy_n = locy + distance * cos(course_rad) / horizontal_scale;

    noise->x = locx_n;
    noise->y = locy_n;


    return {locx = locx_n, locy = locy_n};
}

int submarine_t::get_flooded_compartments() {
    return 0;//stoi(terminal::query("Compartments flooded in sub " + to_string(id) + '-' + name + ':'));
}

float submarine_t::getLocx() const {
    return locx;
}

float submarine_t::getLocy() const {
    return locy;
}

int submarine_t::getId() const {
    return id;
}

const string &submarine_t::getName() const {
    return name;
}

const string &submarine_t::getBoatClass() const {
    return boat_class;
}

int submarine_t::getDirection() const {
    return direction;
}

int submarine_t::getDepth() const {
    return depth;
}

float submarine_t::get_speed() const {
    return distance_3d({locx, locy, depth}, last_position) / TURN_TIME;
}

sound_t submarine_t::get_sound() const {
    return sound;
}

const power_threshold_t &submarine_t::get_power_setting(const int power) const {
    for (auto it = power_thresholds.begin(); it != power_thresholds.end(); it++)
        if (it->generated > power)
            return *it;
    return power_thresholds.back();
}

[[nodiscard]] const speed_setting_t &submarine_t::get_speed_setting(const float distance) const {
    for (auto &speed_record : speeds) {
        if (speed_record.distance >= distance)
            return speed_record;
    }
    return speeds.back();
}

speed_setting_t speed_setting_t::parse_from_line(const std::string &line) {
    speed_setting_t speed_setting;

    auto pos = 0;
    while (pos < line.length() && !is_alphanumeric(line[pos]))
        pos++;

    auto start_pos = pos;
    while (pos < line.length() && line[pos] != ':')
        pos++;

    auto end_pos = pos;

    speed_setting.order = line.substr(start_pos, end_pos - start_pos);

    // Get distance
    while (pos < line.length() && !is_numeric(line[pos]))
        pos++;
    start_pos = pos;
    while (pos < line.length() && !is_alphabetic(line[pos]))
        pos++;
    end_pos = pos;
    speed_setting.distance = stoi(line.substr(start_pos, end_pos - start_pos));

    // Get power token cost
    while (pos < line.length() && !is_numeric(line[pos]))
        pos++;
    start_pos = pos;
    while (pos < line.length() && is_numeric(line[pos]))
        pos++;
    end_pos = pos;
    speed_setting.power = stoi(line.substr(start_pos, end_pos - start_pos));


    // Get power noise
    while (pos < line.length() && !is_numeric(line[pos]))
        pos++;
    start_pos = pos;
    while (pos < line.length() && is_numeric(line[pos]))
        pos++;
    end_pos = pos;
    speed_setting.noise = stoi(line.substr(start_pos, end_pos - start_pos));

    return speed_setting;
}

power_threshold_t power_threshold_t::parse_from_line(const std::string &line) {
    power_threshold_t threshold;

    auto pos = 0;

    // Get upper bound of generated power
    while (pos < line.length() && !is_numeric(line[pos]))
        pos++;
    auto start_pos = pos;
    while (pos < line.length() && is_numeric(line[pos]))
        pos++;
    auto end_pos = pos;
    threshold.generated = stoi(line.substr(start_pos, end_pos - start_pos));

    // Get threshold
    while (pos < line.length() && !is_numeric(line[pos]))
        pos++;
    start_pos = pos;
    while (pos < line.length() && is_numeric(line[pos]))
        pos++;
    end_pos = pos;
    threshold.noise = stoi(line.substr(start_pos, end_pos - start_pos));

    return threshold;
}
