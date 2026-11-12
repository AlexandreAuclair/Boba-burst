
# 1. Force le runtime global dynamique pour tout le monde (Debug = /MDd, Release = /MD)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# 2. NEUTRALISE l'option statique interne de Jolt 
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "Use static runtime" FORCE)

add_subdirectory(./deps/glfw)
add_subdirectory(./deps/glm)
add_subdirectory(./deps/assimp)
add_subdirectory(./deps/JoltPhysics/Build)
add_subdirectory(./deps/json-develop)

find_package(Vulkan REQUIRED)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Boba_burst 
"./src/bb_main.cpp"
"./src/Game/bb_game.cpp"
"./src/Game/bb_game.h"
"./src/Game/bb_camera.h"
"./src/Game/bb_gameState.h"
"./src/Game/bb_gameState.cpp"
"./src/Game/bb_gameManager.h"
"./src/Game/bb_gameManager.cpp"
"./src/Logger/Logger.cpp"
"./src/Logger/Logger.h"
"./src/LevelLoader/bb_levelLoader.h"
"./src/LevelLoader/bb_levelLoader.cpp"
"./src/Actor/bb_actor.h"
"./src/Actor/Character/bb_character.h"
"./src/Actor/Character/bb_character.cpp"
"./src/Actor/Character/bb_bot.h"
"./src/Actor/Character/bb_bot.cpp"
"./src/Actor/Character/bb_player.h"
"./src/Actor/Character/bb_player.cpp"
"./src/Actor/Object/bb_track.h"
"./src/Actor/Object/bb_track.cpp"
"./src/Actor/Object/bb_trackObject.h"
"./src/Actor/Object/bb_trackObject.cpp"
"./src/Actor/Object/bb_bonusObject.h"
"./src/Actor/Object/bb_bonusObject.cpp"
"./src/Events/bb_keyPressedEvent.h"
"./src/EventBus/bb_event.h"
"./src/EventBus/bb_eventBus.h"
"./src/DataStruct/bb_mesh.h"
"./src/DataStruct/bb_section.h"
"./src/DataStruct/bb_sommet.h"
)



set_property(TARGET Boba_burst PROPERTY CXX_STANDARD 20)

set(LINK_LIBRARIES ${Vulkan_LIBRARIES} glfw glm assimp::assimp nlohmann_json Jolt)

target_link_libraries(Boba_burst PRIVATE ${LINK_LIBRARIES})

target_include_directories(Boba_burst PRIVATE ${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})