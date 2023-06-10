cmake_minimum_required(VERSION 3.14)
project(SFMLGame)

set(CMAKE_CXX_STANDARD 17)
include_directories(headers /user/include c:/SFML/include)
if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(CMAKE_PREFIX_PATH C:/)
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

# GAME FILES (Files worked on by us)
set(SOURCE_FILES
        src/main.cpp
        src/Game.cpp src/Game.h

        #PLAYER FILES
        src/Player/Player.cpp src/Player/Player.h

        #GAME OBJECT FILES
        src/GameObject/GameObject.cpp src/GameObject/GameObject.h

        #HEAT SYSTEM FILES
        src/HeatSystem/Coal.cpp src/HeatSystem/Coal.h
        src/HeatSystem/Furnace.cpp src/HeatSystem/Furnace.h
        src/HeatSystem/Thermometer.cpp src/HeatSystem/Thermometer.h

        #LIGHTING FILES
        src/Lighting/LightingSystem.cpp src/Lighting/LightingSystem.h
        src/Lighting/Edge.h
        src/Lighting/Line.h
        src/Lighting/LightSource.cpp
        src/Lighting/LightSource.h

        #NETWORKING FILES
        #src/networking.cpp
        src/Networking/Server.cpp src/Networking/Server.h
        src/Networking/Client.cpp src/Networking/Client.h
        src/Networking/NetworkingPackets.h src/Networking/NetworkingPackets.cpp

        #UI FILES
        src/UIElements/Objective.cpp src/UIElements/Objective.h
        src/UIElements/UiButton.h src/UIElements/UiButton.cpp
        src/UIElements/UISymbol.cpp src/UIElements/UISymbol.h
        src/UIElements/ParallaxBackground.cpp src/UIElements/ParallaxBackground.h
        src/UIElements/ControllerCursor.cpp src/UIElements/ControllerCursor.h
        src/UIElements/Minimap.cpp src/UIElements/Minimap.h

        #AUDIO FILES
        src/Audio/AudioManager.cpp src/Audio/AudioManager.h

        #GAME STATE FILES
        src/Gamestates/GameData.h
        src/Gamestates/GameState.cpp src/Gamestates/GameState.h
        src/Gamestates/Menu.cpp src/Gamestates/Menu.h
        src/Gamestates/Lobby.cpp src/Gamestates/Lobby.h
        src/Gamestates/Gameplay.cpp src/Gamestates/Gameplay.h
        src/Gamestates/GameOver.cpp src/Gamestates/GameOver.h

        #TILE MAP FILES
        src/Tilemap/Tilemap.cpp src/Tilemap/Tilemap.h
        )

# TMX FILES (NOT CREATED BY US)
set(TMX_FILES include/FreeFuncs.cpp
        include/ImageLayer.cpp
        include/LayerGroup.cpp
        include/Map.cpp
        include/miniz.c include/miniz.h
        include/Object.cpp
        include/ObjectGroup.cpp
        include/ObjectTypes.cpp
        include/Property.cpp
        include/TileLayer.cpp
        include/Tileset.cpp
        include/detail/pugixml.cpp
        )
add_executable(SFMLGame ${SOURCE_FILES} ${TMX_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/openal32.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

target_link_libraries (SFMLGame sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)
