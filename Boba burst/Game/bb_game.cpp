#include "bb_game.h"
    //==========================================================================
    //      include externe
    //==========================================================================

    //==========================================================================
    //      include interne
    //==========================================================================
#include "../Logger/Logger.h"

//==========================================================================
//      fonction publique
//==========================================================================

Game::Game(){
    isRunning = false;
    isDebug = false;

    Logger::Log("Game constructor called!");

}

Game::~Game() {
    Logger::Log("Game destructor called!");
}

void Game::Initialize() {
    if (!glfwInit())
    {
        Logger::Err("Failed to initialize glfw.\r\n");
        return;
    }

    if (!glfwVulkanSupported())
    {
        Logger::Err("Vulkan not supported.\r\n");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(1024, 768, "Boba Burst", NULL, NULL);
    if (!window)
    {
        Logger::Err("Failed to create window.\r\n");
        return;
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// Cache le curseur et le verrouille au mileu de la fen�tre
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Game::Run() {
    Setup();
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    Logger::Shutdown();
    //GameManager::Shutdown();

    /*// Pour plus tard pour le profiler
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    */
    glfwDestroyWindow(window);
    glfwTerminate();
}

//==========================================================================
//      fonction privé
//==========================================================================

void Game::Setup() {

}

void Game::ProcessInput() {

}

void Game::Update() {
    glfwPollEvents();
}

void Game::Render() {

}