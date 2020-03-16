//
// Created by james on 02/03/2020.
//

#include "file_handle_t.h"

bool is_alphabetic(char symbol) {
    return symbol >= 'A' && symbol <= 'z';
}

inline bool is_numeric(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool is_alphanumeric(const char symbol) {
    if (symbol >= 'A' && symbol <= 'z')
        return true;
    else
        return is_numeric(symbol);
}


std::string file_handle_t::parse_string_line() {
    if (type == IN) {
        std::string line;
        std::getline(in_handle, line, ':');
        std::getline(in_handle, line, '\n');
        auto pos = 0;
        while (pos < line.length() && !is_alphanumeric(line[pos]))
            pos++;
        return line.substr(pos, line.length() - pos);
    }
    return std::__cxx11::string();
}

int file_handle_t::parse_int_line() {
    return stoi(parse_string_line());
}

std::pair<std::string, std::vector<std::string> > file_handle_t::parse_indented_list() {
    std::pair<std::string, std::vector<std::string>> string_list;
    std::string line;
    if (type == IN){
        std::getline(in_handle, line, ':');
        string_list.first = line;
        std::getline(in_handle, line, '\n');

        char character = in_handle.peek();
        while (in_handle.peek() == '\t'){
            while (in_handle.peek() != '\t')
                in_handle >> character;

            std::getline(in_handle, line, '\n');
            string_list.second.push_back(line);
        }
    }

    return string_list;
}
