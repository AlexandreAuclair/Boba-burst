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

Game::Game() : isDebug(false), isRunning(true){
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
        double elapsed = currentTime - lastTime;
        ProcessInput();
        Update();
        Render();
        lastTime = currentTime;
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
    // Appeler le level Loader
    levelLoader.LoadLevel(1,"level.json");
}

void Game::ProcessInput() {
    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
            // Gérer les clics sur les boutons du menu principal
            // demander a GLFW de montrer la souris
            // Si clic sur "Play" -> stateMachine.TriggerPlay(); cacher la souris avec glfwSetInputMode
            // Si clic sur "Options" -> stateMachine.TriggerOption();
            // Si clic sur "Quit" -> stateMachine.TriggerQuit();
        break;
        case GameState::Playing:
            // Gérer les inputs du joueur
        break;
        case GameState::Pause:
            // Gérer les inputs du menu pause (Reprendre, Menu, Quitter)
        break;
        case GameState::GameOver:
            // Si le joueur clique sur "Rejouer" -> stateMachine.TriggerRestart();
            // Si le joueur clique sur "Menu" -> stateMachine.TriggerBackToMenu();
        break;
        case GameState::Option:
            // Si le joueur clique sur "Retour" -> stateMachine.TriggerBackToMenu();
        break;
        case GameState::Quitting:
            // Si le joueur a appuyer sur QUIT alors on désactive son controleur et on remontre la souris
        break;
    }
}

void Game::Update() {
    glfwPollEvents();
    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
        break;
        case GameState::Playing:
            // Mettre à jour la physique Jolt, les acteurs, la caméra
        break;
        case GameState::Pause:
            // Le jeu est en pause (on ne met pas à jour la physique/le gameplay)
        break;
        case GameState::GameOver:
            // Le jeu devrais retourner a Setup() pour réinitialiser toute les variable du jeu
        break;
        case GameState::Option:
            // Selon les bouton appuyer on doit changer les options Vulkan ici
            // regarder les event qui on été activer
        break;
        case GameState::Quitting:
            // pas besoin que je vous explique ce qu'il faut faire ici
            isRunning = false;
        break;
    }
}

void Game::Render() {
    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
            // Afficher l'écran de début et les options
        break;
        case GameState::Playing:
            // Afficher ce que la camera voit aka. la scène
        break;
        case GameState::Pause:
            // Afficher la scène en plus noir(genre * 0.5 les couleurs)
            // afficher les options du menu Pause
        break;
        case GameState::GameOver:
            // Afficher l'écran de Game Over
            // UI image
        break;
        case GameState::Option:
            // Afficher les options dans le UI
        break;
        case GameState::Quitting:
            // honnêtement je sais pas si on a le temps d'afficher de quoi ici...
        break;
    }
}