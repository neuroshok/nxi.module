module_add(discord)


file(GLOB_RECURSE D_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

target_sources(module_discord INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/discord.cpp ${D_SOURCES})
target_link_libraries(module_discord INTERFACE D:/project/tests/discord_api/discord_game_sdk.dll.lib)