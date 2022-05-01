cmake_minimum_required(VERSION 3.23)

project(example.rust)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_target(mod_rust.example)

# module_add(mod_rust.example "rust")

add_custom_command(
    TARGET mod_rust.example PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${NXI_BIN_ROOT}/nxi.lib ./nxi.lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${NXI_MODULE_ROOT}/static/nxi.module/example.rust/ ./
    COMMAND cargo
    ARGS build --release
)

add_custom_command(
    TARGET mod_rust.example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/module/static/nxi.module/example.rust/target/release/mod_rust.dll ${NXI_BIN_ROOT}/nxi/module/dynamic/mod_rust.dll
)

add_dependencies(mod_rust.example nxi_deploy)