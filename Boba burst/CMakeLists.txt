
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

list(APPEND SHADERS
    ./shaders/global.vert
    ./shaders/global.frag

    # Shaders pour l'UI de test
    ./shaders/ui.vert
    ./shaders/ui.frag
    ./shaders/skybox.frag
)
set(SHADER_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders_out)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Boba_burst 
"./src/bb_main.cpp"
"./src/stb_image.h"
"./src/Game/bb_game.cpp"
"./src/Game/bb_game.h"
"./src/Game/bb_camera.h"
"./src/Game/bb_camera.cpp"
"./src/Game/bb_gameState.h"
"./src/Game/bb_gameState.cpp"
"./src/Game/bb_gameManager.h"
"./src/Game/bb_gameManager.cpp"
"./src/Game/PhysicsWorld.h"
"./src/Game/PhysicsWorld.cpp"
"./src/Logger/Logger.cpp"
"./src/Logger/Logger.h"
"./src/LevelLoader/bb_levelLoader.h"
"./src/LevelLoader/bb_levelLoader.cpp"
"./src/Actor/bb_actor.h"
"./src/Actor/Character/bb_character.h"
"./src/Actor/Character/bb_character.cpp"
"./src/Actor/Character/bb_inputManager.h"
"./src/Actor/Character/bb_inputManager.cpp"
"./src/Actor/Character/bb_bot.h"
"./src/Actor/Character/bb_bot.cpp"
"./src/Actor/Character/bb_player.h"
"./src/Actor/Character/bb_player.cpp"
"./src/Actor/Object/bb_track.h"
"./src/Actor/Object/bb_track.cpp"

"./src/Actor/Object/bb_goal.h"
"./src/Actor/Object/bb_goal.cpp"

"./src/Actor/Object/bb_trackObject.h"
"./src/Actor/Object/bb_trackObject.cpp"
"./src/Actor/Object/bb_bonusObject.h"
"./src/Actor/Object/bb_bonusObject.cpp"
"./src/Actor/Object/ObjectTypes.h"
"./src/Actor/Object/obstacle.h"
"./src/Actor/Object/obstacle.cpp"
"./src/Events/bb_keyPressedEvent.h"
"./src/EventBus/bb_event.h"
"./src/EventBus/bb_eventBus.h"
"./src/DataStruct/bb_mesh.h"
"./src/DataStruct/bb_section.h"
"./src/DataStruct/bb_sommet.h"
"./src/DataStruct/bb_cameraConfig.h"
"./src/Scenes/bb_gameScene.h"
"./src/Scenes/bb_gameScene.cpp"
"./src/VulkanContext/bb_vulkanContext.h"
"./src/VulkanContext/bb_vulkanContext.cpp"
"./src/VulkanRenderer/bb_vulkanRenderer.h"
"./src/VulkanRenderer/bb_vulkanRenderer.cpp"
"./src/ModelImporter/bb_modelImporter.h"
"./src/ModelImporter/bb_modelImporter.cpp"

# Ajout des controller => Deprecie
#"./src/Controller/Controller.h"
#"./src/Controller/Controller.cpp"

# Ajout des renderer de l'UI
"./src/UIRenderer/UIRenderer.h"
"./src/UIRenderer/UIRenderer.cpp"
"./src/UIRenderer/PauseMenuGeometry.h"
"./src/UIRenderer/UIManager.h"
"./src/UIRenderer/UIManager.cpp"

"./src/Events/bb_onCollisionEvent.h"
"./src/Events/bb_gameMovementEvent.h"
"./src/Events/bb_winEvent.h"

"./src/Utilities/utilities.h"
"./src/Utilities/bb_frustum.h"
)

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_OUT_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} --target-env=vulkan1.3 ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} -o ${SHADER_OUT_DIR}/${FILENAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILENAME}")
    list(APPEND SPV_SHADERS ${SHADER_OUT_DIR}/${FILENAME}.spv)
endForeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

set_property(TARGET Boba_burst PROPERTY CXX_STANDARD 20)

set(LINK_LIBRARIES ${Vulkan_LIBRARIES} glfw glm assimp::assimp nlohmann_json Jolt)

target_link_libraries(Boba_burst PRIVATE ${LINK_LIBRARIES})

target_include_directories(Boba_burst PRIVATE ${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(Boba_burst shaders)
