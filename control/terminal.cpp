//
// Created by james on 02/03/2020.
//

#include <iostream>
#include "terminal.h"

using namespace std;

string terminal::query(const string& question_string) {
    cout << question_string;
    string answer_string;
    cin >> answer_string;
    return answer_string;
}
