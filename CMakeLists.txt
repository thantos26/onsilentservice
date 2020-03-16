cmake_minimum_required(VERSION 3.14)
project(OnSilentService)

set(CMAKE_CXX_STANDARD 17)

add_executable(Deeptrouble control/main.cpp control/terminal.cpp control/terminal.h control/game_model_t.cpp control/game_model_t.h control/submarine_t.cpp control/submarine_t.h control/file_handle_t.cpp control/file_handle_t.h control/noise_source_t.cpp control/noise_source_t.h control/mapping.cpp control/mapping.h)