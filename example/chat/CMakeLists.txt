
file(GLOB ALL_SRC_FILE *.cpp)

foreach(SRC_FILE ${ALL_SRC_FILE})
    get_filename_component(_target ${SRC_FILE} NAME_WE)
    add_executable(${_target} ${SRC_FILE})
    target_link_libraries(${_target} asio)
endforeach()
