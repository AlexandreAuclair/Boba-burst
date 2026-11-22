#include "bb_game.h"
//==========================================================================
//      include externe
//==========================================================================

//==========================================================================
//      include interne
//==========================================================================
#include "../Logger/Logger.h"
#include "../Scenes/bb_gameScene.h"
#include "../Controller/Controller.h"


//==========================================================================
//      fonction publique
//==========================================================================

Game::Game() : isDebug(false), isRunning(true){
    Logger::Log("Game constructor called!");
    gameEventBus = std::make_unique<EventBus>();
}

Game::~Game() {
    Logger::Log("Game destructor called!");
}

bool Game::Initialize() {
    if (!glfwInit())
    {
        Logger::Err("Failed to initialize glfw.\r\n");
        return false;
    }

    if (!glfwVulkanSupported())
    {
        Logger::Err("Vulkan not supported.\r\n");
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(1024, 768, "Boba Burst", NULL, NULL);
    if (!window)
    {
        Logger::Err("Failed to create window.\r\n");
        return false;
    }

    //// Donner la fenêtre au controller
    //controller.SetWindow(window);

    context.CreateInstance();
	context.CreateDevice();
    context.window = window;

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkResult result = glfwCreateWindowSurface(context.GetInstance(), window, NULL, &surface);
    if(result != VK_SUCCESS){
        Logger::Err("Failed to create window surface.\r\n");
        return false;
    }

	context.SetSurface(surface);
	context.CreateSwapChain();

    renderer = std::make_unique<VulkanRenderer>(context);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// Cache le curseur et le verrouille au mileu de la fen�tre
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    /// Nous assure que le bus est vide au setup
    gameEventBus->Reset();

    m_physicsWorld = make_unique<PhysicsWorld>(gameEventBus);

    return true;
}

void Game::Run() {
   
    // Setup l'ensemble du jeu 
    Setup();

    // Récupérer le temps 
    double lastTime = glfwGetTime();


    while (isRunning && !glfwWindowShouldClose(window)) {
        
        // 1. Actualiser les événements
        glfwPollEvents();

        // 2. Calculer le deltaTime
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;
        lastTime = currentTime;
        millisecsPreviousFrame = dt;
        //// Ferme la fenêtre si on appuie sur Echap
        //if (controller.IsKeyPressed(GLFW_KEY_ESCAPE)) {
        //    glfwSetWindowShouldClose(window, GLFW_TRUE);
        //}
        
        // 3. Traiter les inputs
        ProcessInput();

        // 4. Mettre à jour la logique
        Update(dt);

        // 5. Dessiner la frame
        Render(dt);
    }
}

void Game::Destroy() {
    Logger::Shutdown();
    //GameManager::Shutdown();
    levelLoader.FreeModelAndTextures();
    /*// Pour plus tard pour le profiler
    ImGui::DestroyContext();
    */

    // Fin de vie du programme
    context.GetQueue().waitIdle();
    glfwDestroyWindow(window);
    glfwTerminate();
}

//==========================================================================
//      fonction privé
//==========================================================================

void Game::Setup() {
    // Appeler le level Loader
    if(!levelLoader.LoadLevel("level1.json")){
        Logger::Err("Could not load level1.json");
    }
    else{
        Logger::Log("level1.json successfully loaded");
    }

    // setter la 1er scene
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

void Game::ProcessInput()
{
    switch (stateMachine.GetCurrentState())
    {
    case GameState::Menu:
        // Gérer les clics sur les boutons du menu principal
        // demander a GLFW de montrer la souris
        // Si clic sur "Play" -> stateMachine.TriggerPlay(); cacher la souris avec glfwSetInputMode
        // Si clic sur "Options" -> stateMachine.TriggerOption();
        // Si clic sur "Quit" -> stateMachine.TriggerQuit();
        break;
    case GameState::Playing:
    {
        // Gérer les inputs du joueur
        // Ask all players to check input
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::for_each(GameManager::get().players.begin(), GameManager::get().players.end(), [](auto &player)
                      { 
                player.get()->controlManager.CheckInput(); 
            });
    }
    break;
    case GameState::Pause:
    {
        // Gérer les inputs du menu pause (Reprendre, Menu, Quitter)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::for_each(GameManager::get().players.begin(), GameManager::get().players.end(), [](auto &player)
                      { player.get()->controlManager.CheckInput(); });
    }
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

/*void Game::ProcessInput(){

    bool tabPressed = controller.IsKeyPressed(GLFW_KEY_TAB);

    if (tabPressed && !tabWasPressed) {
        switch (stateMachine.GetCurrentState()) {
            case GameState::Playing : 
                Logger::Log("Tab pressee - Entree en pause");
                stateMachine.TriggerPause();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
        
            case GameState::Pause : 
                Logger::Log("Tab pressee - Retour au jeu");
                stateMachine.TriggerPlay();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;

            default : 
                Logger::Err("State incohérent avec la touche"); 
        }

    }
    tabWasPressed = tabPressed;

/*    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
            // Gérer les clics sur les boutons du menu principal
            // demander a GLFW de montrer la souris
            // Si clic sur "Play" -> stateMachine.TriggerPlay(); cacher la souris avec glfwSetInputMode
            // Si clic sur "Options" -> stateMachine.TriggerOption();
            // Si clic sur "Quit" -> stateMachine.TriggerQuit();
        break;
        case GameState::Playing:
            // Gérer les inputs du joueur
            if (controller.IsKeyPressed(GLFW_KEY_TAB)) {
                Logger::Log("Touche Tab appuyée - Entrée mode menu");
                stateMachine.TriggerPause();
            }
            
        break;


        case GameState::Pause:
            // Gérer les inputs du menu pause (Reprendre, Menu, Quitter)

            // TAB : Quitter la pause, retour au jeu
            // Voir s'il y aura besoin d'un système pour éviter les doubles inputs
            if (controller.IsKeyPressed(GLFW_KEY_TAB)) {
                Logger::Log("Touche Tab appuyée - Retour mode jeu");
                stateMachine.TriggerPlay();
            }

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
}*/

void Game::Update(double dt) {
    glfwPollEvents();
    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
        break;
    case GameState::Playing:
        // Mettre à jour la physique Jolt, les acteurs, la caméra
	    scene->Update(dt);
        // La physique de Jolt est mise à jours et les événement de collision sont lancer.
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
        for(auto& character : GameManager::get().characters)
        {
            character->Update(millisecsPreviousFrame); 
        }

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

void Game::Render(double dt) {


    switch (stateMachine.GetCurrentState()) {
        case GameState::Menu:
            // Afficher l'écran de début et les options
        break;
        case GameState::Playing:
            // Afficher ce que la camera voit aka. la scène
            renderer->Draw(false);
        break;
        case GameState::Pause:
            // Afficher la scène en plus noir(genre * 0.5 les couleurs)
            // afficher les options du menu Pause
            renderer->Draw(true);
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

/// @brief Quand un événement de touche est lancé, s'il sagit de la touche ESC, trigger les changements états et les événements approprié selon l'états actuel (vers la pause ou vers la reprise)
/// @param event La touche présentement active
void Game::HandlePauseEvent(KeyPressedEvent &event)
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

   
