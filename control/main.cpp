#include <iostream>
#include "game_model_t.h"

#include <cfenv>
#pragma STDC FENV_ACCESS ON

int main() {

    fesetround(FE_TONEAREST); // Not convinced this helps

    game_model_t model;

    model.import_submarine("../boats/agile.txt");
    model.import_submarine("../boats/vigilant.txt");
    //model.import_submarine("../boats/formidable.txt");
    model.import_noise("../noises/whales.txt");
    model.import_noise("../noises/volcanic_vent.txt");

    model.export_submarine(0, "vigilant_copy.txt");

    model.enter_movements();

    std::cout << model.generate_passive_sonar_report(0) << std::endl;
    std::cout << model.generate_passive_sonar_report(1) << std::endl;

    return 0;
}