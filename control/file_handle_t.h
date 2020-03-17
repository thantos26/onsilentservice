//
// Created by james on 02/03/2020.
//

#ifndef DEEPTROUBLE_FILE_HANDLE_T_H
#define DEEPTROUBLE_FILE_HANDLE_T_H

#include <fstream>
#include <vector>

bool is_alphabetic(char symbol);
bool is_numeric(char symbol);
bool is_alphanumeric(char symbol);

struct file_handle_t {
    const enum io_t {
        IN, OUT
    } type;

    std::ifstream in_handle;
    std::ofstream out_handle;

    file_handle_t(const std::string &path, const io_t io_type) : type(io_type) {
        if (type == IN) in_handle.open(path); else out_handle.open(path);
    }

    ~file_handle_t(){
        if (type == IN) in_handle.close(); else out_handle.close();
    }

    int parse_int_line();
    std::string parse_string_line();
    std::pair<std::string, std::vector<std::string> > parse_indented_list();

};


#endif //DEEPTROUBLE_FILE_HANDLE_T_H
