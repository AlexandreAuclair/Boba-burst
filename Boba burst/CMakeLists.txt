
add_subdirectory(./deps/glfw)
add_subdirectory(./deps/glm)

find_package(Vulkan REQUIRED)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Boba_burst 
"bb_main.cpp"
"./Game/bb_game.cpp"
"./Game/bb_game.h"
"./Game/bb_camera.h"
"./Logger/Logger.cpp"
"./Logger/Logger.h"
)

set_property(TARGET Boba_burst PROPERTY CXX_STANDARD 20)

set(LINK_LIBRARIES ${Vulkan_LIBRARIES} glfw glm)

target_link_libraries(Boba_burst PRIVATE ${LINK_LIBRARIES})

target_include_directories(Boba_burst PRIVATE ${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})