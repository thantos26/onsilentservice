//
// Created by james on 02/03/2020.
//

#ifndef DEEPTROUBLE_GAME_MODEL_T_H
#define DEEPTROUBLE_GAME_MODEL_T_H

#include <map>
#include <set>
#include <vector>
#include <utility>

#include "submarine_t.h"
#include "noise_source_t.h"

class game_model_t {

    int game_time = 0;

    std::map<int, submarine_t> boats;
    std::vector<std::vector<sounding_t> > game_map;
    std::set<std::shared_ptr<noise_source_t> > noise_map;


    [[nodiscard]] std::pair<int, int> snap_ref_to_grid(float x, float y) const;
    [[nodiscard]] std::pair<int, int> snap_sub_to_grid(int id) const;

    std::optional<submarine_t> get_sub_from_ref(int x, int y);

public:
    void import_submarine(const std::string& file_path);
    void export_submarine(int id, const std::string& file_path);

    void import_noise(const std::string& file_path);

    void enter_movements();
    [[nodiscard]] std::string generate_passive_sonar_report(int sub_id);

    void export_report(const std::string &file_path, int sub_id);

};


#endif //DEEPTROUBLE_GAME_MODEL_T_H
