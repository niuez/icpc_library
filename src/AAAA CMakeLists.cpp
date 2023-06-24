add_definitions(-DLOCAL)
add_compile_options(-fsanitize=undefined,address)
add_executable(projectname main.cpp)
target_link_libraries(projectname -fsanitize=undefined,address)
