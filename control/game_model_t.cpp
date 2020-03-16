//
// Created by james on 02/03/2020.
//

#include "terminal.h"
#include "game_model_t.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

/**
 * Enter the movement orders for each of the submarines, calculate their new position.
 * Entry should be submitted in any order. Data should be written back to files.
 */
void game_model_t::enter_movements() {
    cout << "Enter movement for each submarine:" << endl;
    for (auto &submarine : boats) {
        cout << submarine.second.getId() << '-' << submarine.second.getName() << ':' << endl;
        submarine.second.move(
                stof(terminal::query("\tdistance:")),
                stoi(terminal::query("\tcourse:")),
                stoi(terminal::query("\tascent:")), 0);

        // TODO collision checks

        cout << "Moved to (" << submarine.second.getLocx() << ", " << submarine.second.getLocy() << ", "
             << submarine.second.getDepth() << ')' << endl << endl;
    }
}

void game_model_t::import_submarine(const std::string &file_path) {
    submarine_t sub(file_path);

    // TODO insert bounds checks

    auto id = sub.getId();

    auto noise = std::make_shared<noise_source_t>(sub);
    boats.insert({id, sub});

    noise->submarine = &boats[id];
    noise_map.insert(noise);
    boats[id].noise = noise;
}

void game_model_t::export_submarine(const int id, const std::string &file_path) {
    auto &submarine = boats[id];
    submarine.export_sub(file_path);
}

void game_model_t::import_noise(const std::string &file_path) {
    noise_map.insert(std::make_shared<noise_source_t>(file_path));
}

pair<int, int> game_model_t::snap_ref_to_grid(float x, float y) const {
    return {round(x), round(y)};
}

pair<int, int> game_model_t::snap_sub_to_grid(int id) const {
    // TODO

    return {0, 0};
}

optional<submarine_t> game_model_t::get_sub_from_ref(int x, int y) {
    auto it = find_if(boats.begin(), boats.end(), [=](const auto &sub) {
        return snap_ref_to_grid(sub.second.getLocx(), sub.second.getLocy()) == pair<int, int>({x, y});
    });
    if (it == boats.begin())
        return it->second;

    return nullopt;
}

std::string game_model_t::generate_passive_sonar_report(int sub_id) {
    const auto &submarine = boats[sub_id];
    std::string report =
            "Passive sonar report for submarine " + to_string(sub_id) + '-' + submarine.getName() + " at time " +
            to_string(game_time) + '\n';

    for (auto &noise : noise_map) {

        // Don't report one's own noise
        if (noise->type == noise_source_t::noise_type_t::SUBMARINE)
            if (noise->submarine->getId() == submarine.getId())
                continue;

        auto noise_report = noise->get_report({submarine.getLocx(), submarine.getLocy()});

        report.append(noise_report.description);

        auto bearing = noise_report.direction - submarine.getDirection();
        if (bearing == 0)
            report.append(" ahead ");
        else if (bearing == 180)
            report.append(" astern ");
        else {
            report.append(" bearing ");
            report.append(((bearing > 0) ? "green " : "red ") + to_string(abs(bearing)));
        }

        if (noise_report.speed != 0) {
            report.append(" moving at speed " + to_string(noise_report.speed) + " on heading " +
                          to_string(noise_report.heading));
        }

        report.append("\n");
    }

    return report;
}
