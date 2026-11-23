#include "bb_game.h"

#include <cstdint>
#include <iostream>
#include <memory>

#include "../Logger/Logger.h"
#include "../Scenes/bb_gameScene.h"
#include "../UIRenderer/UIManager.h"

// ============================================================================
// Construction et initialisation
// ============================================================================

Game::Game()
    : isRunning(true),
    isDebug(false),
    window(nullptr),
    gameEventBus(std::make_unique<EventBus>())
{
    Logger::Log("Game constructor called!");
}

Game::~Game()
{
    Logger::Log("Game destructor called!");
}

bool Game::Initialize()
{
    // Initialiser GLFW et vérifier que Vulkan est disponible.
    if (!glfwInit())
    {
        Logger::Err("Failed to initialize GLFW.");
        return false;
    }

    if (!glfwVulkanSupported())
    {
        Logger::Err("Vulkan not supported.");
        return false;
    }

    // Vulkan effectue le rendu : GLFW ne crée pas de contexte OpenGL.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(1024, 768, "Boba Burst", nullptr, nullptr);

    if (!window)
    {
        Logger::Err("Failed to create window.");
        return false;
    }

    // Préparer les ressources Vulkan et la surface associée à la fenêtre.
    context.CreateInstance();
	  context.CreateDevice();
    context.window = window;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    const VkResult result = glfwCreateWindowSurface(
        context.GetInstance(), window, nullptr, &surface);

    if (result != VK_SUCCESS)
    {
        Logger::Err("Failed to create window surface.");
        return false;
    }

    context.SetSurface(surface);
    context.CreateSwapChain();
    renderer = std::make_unique<VulkanRenderer>(context);

    // Autoriser le mouvement brut pour la caméra lorsque le curseur est capturé.
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

   

    return true;
}

// ============================================================================
// Boucle principale et arrêt
// ============================================================================

void Game::Run()
{
    Setup();
    if (!isRunning)
    {
        return; // Le chargement initial a échoué.
    }

    // Initialiser le curseur selon l'état initial, sans attendre un appui sur Échap.
    const int initialCursorMode =
        stateMachine.GetCurrentState() == GameState::Playing
        ? GLFW_CURSOR_DISABLED
        : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(window, GLFW_CURSOR, initialCursorMode);

    double lastTime = glfwGetTime();

    while (isRunning && !glfwWindowShouldClose(window))
    {
        // 1. Récolter une seule fois les événements GLFW de la frame.
        glfwPollEvents();

        // 2. Calculer le temps écoulé depuis la frame précédente (en secondes).
        const double currentTime = glfwGetTime();
        const double dt = currentTime - lastTime;
        lastTime = currentTime;
        millisecsPreviousFrame = static_cast<float>(dt);

        // 3. Entrées -> simulation -> rendu.
        ProcessInput();
        Update(dt);
        Render(dt);
    }
}

void Game::Destroy()
{
    // Attendre la fin des commandes GPU avant de libérer les ressources utilisées.
    context.GetQueue().waitIdle();
    levelLoader.FreeModelAndTextures();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    Logger::Shutdown();
}

// ============================================================================
// Préparation de la scène et des abonnements aux événements
// ============================================================================

void Game::Setup() {
     // Créer le bus d'événements et le monde physique.
     gameEventBus->Reset();
     m_physicsWorld = std::make_unique<PhysicsWorld>(gameEventBus);
    // Appeler le level Loader
    if(!levelLoader.LoadLevel("level1.json")){
        Logger::Err("Could not load level1.json");
        isRunning = false;
        return;
    }
    Logger::Log("level1.json successfully loaded");

    auto& gameManager = GameManager::get();
    if (gameManager.players.empty() || gameManager.characters.empty())
    {
        Logger::Err("Level has no player or character.");
        isRunning = false;
        return;
    }

    // La scène fournit les éléments 3D à dessiner par VulkanRenderer.
    scene = std::make_shared<GameScene>(context);
    renderer->SetScene(scene);

    //GameManager::get().players.back()->Init(gameEventBus, window, m_physicsWorld->GetBodyInterface());
    gameEventBus->SubscribeToEvent<KeyPressedEvent>(this, &Game::HandlePauseEvent);
    GameManager::get().characters.front()->SetFollowTarget(GameManager::get().players.front().get());
    for (auto& p : GameManager::get().players) {
        p->SetControlManager(InputManager(gameEventBus, window));
    }

    Logger::Log("Setup : actorsById.size() = " + std::to_string(GameManager::get().actorsById.size()));
    auto& gm = GameManager::get();
    Logger::Log("actorsById=" + std::to_string(gm.actorsById.size())
        + " trackObjects=" + std::to_string(gm.trackObjects.size())
        + " bonusObjects=" + std::to_string(gm.bonusObjects.size())
        + " obstacles=" + std::to_string(gm.obstacles.size()));

    for (auto &a : GameManager::get().actorsById) {
        a.second->Init(gameEventBus, m_physicsWorld->GetBodyInterface());
    }
    

    

}

// ============================================================================
// Entrées : une seule lecture par frame, puis clics du menu de pause
// ============================================================================

void Game::ProcessInput()
{
    auto& players = GameManager::get().players;
    if (players.empty())
    {
        return;
    }

    // Pour ce prototype, on réutilise le contrôleur du joueur initialisé dans Setup()
    auto& inputManager = players.back()->controlManager;

    switch (stateMachine.GetCurrentState())
    {
    case GameState::Menu:
        // À compléter : interactions du menu principal.
        break;

    case GameState::Playing:
        // Les entrées du joueur ont déjà été lues par CheckInput()
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        std::for_each(GameManager::get().players.begin(), GameManager::get().players.end(), [](auto& player)
        {
            player.get()->controlManager.CheckInput();
        });
        break;

    case GameState::Pause:
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::for_each(GameManager::get().players.begin(), GameManager::get().players.end(), [](auto& player)
            { player.get()->controlManager.CheckInput(); });

        // Le clic doit être un nouvel appui gauche, pas un bouton maintenu
        if (!renderer || !inputManager.WasLeftMouseClicked())
        {
            break;
        }

        // Position du curseur en coordonnées de fenêtre GLFW.
        double mouseX = 0.0;
        double mouseY = 0.0;
        inputManager.GetMousePosition(mouseX, mouseY);

        int windowWidth = 0;
        int windowHeight = 0;
        inputManager.GetWindowSize(windowWidth, windowHeight);

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        inputManager.GetFramebufferSize(framebufferWidth, framebufferHeight);

        if (windowWidth <= 0 || windowHeight <= 0 ||
            framebufferWidth <= 0 || framebufferHeight <= 0)
        {
            break;
        }

        // Convertir la souris dans l'espace normalisé [0, 1].
        const float normalizedX = static_cast<float>(mouseX / windowWidth);
        const float normalizedY = static_cast<float>(mouseY / windowHeight);

        // Le rendu et le test de clic doivent utiliser les mêmes demi-dimensions.
        const UIPushConstants menuSize = renderer->GetPauseMenuSize(
            static_cast<uint32_t>(framebufferWidth),
            static_cast<uint32_t>(framebufferHeight));

        if (m_uiManager.IsInsideMenu(
            normalizedX,
            normalizedY,
            menuSize.halfWidth,
            menuSize.halfHeight))
        {
            std::cout << "CLIC SUR MENU\n";
        }

        const UIAction action = m_uiManager.GetClickedAction(normalizedX, normalizedY, menuSize.halfWidth, menuSize.halfHeight);

        switch (action) {

        case UIAction::QUIT:
            std::cout << "Bouton QUITTER" << std::endl;
            break;

        case UIAction::RESUME:
            std::cout << "Bouton CONTINUER" << std::endl;
            gameEventBus->EmitEvent<KeyPressedEvent>(GLFW_KEY_ESCAPE,0,0);
            break;

        case UIAction::RESTART:
            std::cout << "Bouton RECOMMENCER" << std::endl;
            GameManager::get().FreeThisBitch();
            RestartGame();
            stateMachine.TriggerPlay();
            break;

        case UIAction::NONE:
            break;
        }
       
        break;          // break final pour sortir du switch(gameState)
    }


    case GameState::GameOver:
        // À compléter : relancer une partie ou revenir au menu.
        break;

    case GameState::Option:
        // À compléter : interactions de l'écran d'options.
        break;

    case GameState::Quitting:
        break;
    }
}

// ============================================================================
// Mise à jour du jeu
// ============================================================================

void Game::Update(double dt)
{
    switch (stateMachine.GetCurrentState())
    {
    case GameState::Menu:
        break;

    case GameState::Playing:
    {
        // Seul cet état fait avancer la simulation et les objets du jeu.
        scene->Update(dt);
        m_physicsWorld->Update(millisecsPreviousFrame);

        for(auto& track : GameManager::get().tracks){
            track->UpdateFromPhysics(m_physicsWorld->GetBodyInterface());
        }
        for(auto& obstacle : GameManager::get().obstacles){
            obstacle->UpdateFromPhysics(m_physicsWorld->GetBodyInterface());
        }
        for(auto& player : GameManager::get().players)
        {
            player->UpdateFromPhysics(m_physicsWorld->GetBodyInterface());
            player->Update(millisecsPreviousFrame);
        }

        for (auto& character : GameManager::get().characters)
        {
            character->Update(millisecsPreviousFrame);
        }
        break;
    }

    case GameState::Pause:
        // Ne pas avancer la physique ni le gameplay pendant la pause.
        break;

    case GameState::GameOver:
        break;

    case GameState::Option:
        break;

    case GameState::Quitting:
        isRunning = false;
        break;
    }
}

// ============================================================================
// Rendu : afficher la scène seule ou avec l'interface de pause
// ============================================================================

void Game::Render(double /*dt*/)
{
    switch (stateMachine.GetCurrentState())
    {
    case GameState::Menu:
        // À compléter : affichage du menu principal.
        break;

    case GameState::Playing:
        if(GameManager::get().goal->goalWasReach)
            renderer->Draw(UIScreen::Victory);
        else
            renderer->Draw(UIScreen::None); // Scène 3D seule.
        break;

    case GameState::Pause:
        renderer->Draw(UIScreen::Pause); // Scène 3D + PNG du menu de pause.
        break;

    case GameState::GameOver:
        break;

    case GameState::Option:
        break;

    case GameState::Quitting:
        break;
    }
}

// ============================================================================
// Événement de pause (logique conservée : touche Échap uniquement)
// ============================================================================

/// @brief Avec Échap, reprendre le jeu s'il est en pause, sinon déclencher la pause.
/// @param event Événement clavier reçu depuis l'EventBus.
void Game::HandlePauseEvent(KeyPressedEvent& event)
{
    if (event.keyId != GLFW_KEY_ESCAPE)
    {
        return;
    }

    if (stateMachine.GetCurrentState() == GameState::Pause)
    {
        stateMachine.TriggerResume();
        gameEventBus.get()->EmitEvent<PauseEvent>(false);
    }
    else
    {
        stateMachine.TriggerPause();
        gameEventBus.get()->EmitEvent<PauseEvent>(true);
    }
}

void Game::RestartGame()
{
    gameEventBus->Reset();
    m_physicsWorld = std::make_unique<PhysicsWorld>(gameEventBus);
    // Appeler le level Loader
    if (!levelLoader.LoadLevel("level1Restart.json")) {
        Logger::Err("Could not load level1Restart.json");
        isRunning = false;
        return;
    }
    Logger::Log("level1.json successfully loaded");
    GameManager::get().goal->goalWasReach = false;
    GameManager::get().characters.front()->SetFollowTarget(GameManager::get().players.front().get());
    for (auto& p : GameManager::get().players) {
        p->SetControlManager(InputManager(gameEventBus, window));
    }
    for (auto& a : GameManager::get().actorsById) {
        a.second->Init(gameEventBus, m_physicsWorld->GetBodyInterface());
    }
    gameEventBus->SubscribeToEvent<KeyPressedEvent>(this, &Game::HandlePauseEvent);
}
