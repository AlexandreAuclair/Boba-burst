
add_subdirectory(./deps/glfw)
add_subdirectory(./deps/glm)
add_subdirectory(./deps/assimp)
add_subdirectory(./deps/JoltPhysics/Build)

find_package(Vulkan REQUIRED)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Boba_burst 
"./src/bb_main.cpp"
"./src/Game/bb_game.cpp"
"./src/Game/bb_game.h"
"./src/Game/bb_camera.h"
"./src/Game/bb_gameState.h"
"./src/Logger/Logger.cpp"
"./src/Logger/Logger.h"
"./src/Actor/bb_actor.h"
"./src/Character/bb_character.h"
"./src/Character/bb_bot.h"
"./src/Character/bb_player.h"
"./src/Object/bb_track.h"
"./src/Object/bb_track.cpp"
"./src/Object/bb_trackObject.h"
"./src/Object/bb_bonusObject.h"
"./src/Events/bb_keyPressedEvent.h"
"./src/EventBus/bb_event.h"
"./src/EventBus/bb_eventBus.h"
"./src/DataStruct/bb_mesh.h"
"./src/DataStruct/bb_section.h"
"./src/DataStruct/bb_sommet.h"
)

set_property(TARGET Boba_burst PROPERTY CXX_STANDARD 20)

set(LINK_LIBRARIES ${Vulkan_LIBRARIES} glfw glm assimp::assimp Jolt)

target_link_libraries(Boba_burst PRIVATE ${LINK_LIBRARIES})

target_include_directories(Boba_burst PRIVATE ${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})