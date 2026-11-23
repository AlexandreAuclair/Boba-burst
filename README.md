# Projet de jeu Boba-burst

> **Un projet captivant développé avec passion et un travail d'équipe hors pair !**


##  📝À propos du projet

**Buba-burst** est un jeu dynamique conçu pour offrir une expérience fluide, amusant et engageante. Ce projet démontre notre capacité à collaborer efficacement, à concevoir des fonctionnalités robustes et à offrir une interface propre et réactive dans le contexte du programme DDJV de l'université de Sherbrooke.

---

## ⚙️Technologies & Outils 

- **Langage principal** : C++ / CMake
- **Framework & Libraries** : Vulkan / Jolt / glm / GLFW
- **Gestion de version & Collaboration** : Git, GitHub, Pull Requests & Code Reviews

---

## 👥 L'Équipe derrière le projet

Ce projet est le résultat d'une collaboration serrée où chacun a apporté son expertise et son énergie :

| Membre | Rôle & Contributions | GitHub |
| :--- | :--- | :--- |
| **Alexandre Auclair** | Lead Dev / Architecte & Gameplay | [@AlexandreAuclair](https://github.com/AlexandreAuclair) |
| **Samuel Boisvert** | Lead Dev / Architecte & Gameplay | [@greenwood0301](https://github.com/greenwood0301) |
| **Candice Lamy** | Lead Dev / Architecte & Gameplay | [@celinda42](https://github.com/celinda42) |
| **Ugo Berton** | Lead Dev / Architecte & Gameplay | [@Uberton123](https://github.com/Uberton123) |

> *"Seul on va plus vite, ensemble on va plus loin !"* 🚀

---

## 🛠️ Description de l'implémentation

En général, nous avons respecté les fondations établies dans le GDD. Cependant, comme dans tout projet, les premières implémentations nous ont amenés à apporter certains ajustements d'architecture.

### Chargement des niveaux et rendu 

Chargement dynamique (``LevelLoader``) : Nous avons ajouté la classe ``LevelLoader`` afin d'instancier en mémoire les éléments d'un niveau au démarrage. En s'appuyant sur un fichier *.json* décrivant la scène, cette approche permet de modifier les objets d'un niveau sans avoir à recompiler le projet.


Rendu 3D et interface 2D : Nous avons mis en place l'architecture de rendu Vulkan à travers les classes ``VulkanContext``, ``VulkanRenderer`` et ``Scene``. Pour le menu 2D, nous avons choisi de créer une classe dédiée, ``UIRenderer``, distincte du rendu 3D.

### Chargement des niveaux et rendu 

Isolation de la physique (``PhysicWorld``) : Tous les appels à Jolt sont désormais regroupés dans une classe ``PhysicWorld``. À l'image du rendu, cette centralisation isole la dépendance externe, facilite le contrôle de la simulation physique et simplifie les évolutions futures.

Système d'événements (``EventBus``) : Nous avons simplifié la gestion des événements grâce à un ``EventBus`` unique reposant sur le patron Publisher-Subscriber. Chaque module peut s'abonner à un type de message ou en publier. Comme tous les événements héritent d'une classe de base et transportent leurs arguments, nous avons pu réduire considérablement le nombre de classes d'événements nécessaires.

### Conception des niveaux

- **Détection de fin de partie** : Nous avons ajouté une spécification dédiée aux buts afin qu'un détecteur puisse identifier clairement la fin de la partie.

- **Simplification de la piste** : Nous avons abandonné la notion de face pour les morceaux de piste, une mécanique qui ajoutait trop de complexité pour peu de valeur ajoutée.

### Boucle de jeu et cycle d'initialisation 

Enfin, nous avons profondément revu la boucle de jeu. Les classes ``Game`` et ``GameManager`` sont implémentées comme des singletons afin d'assurer un contrôle centralisé :

- ``Game`` orchestre l'exécution du programme. Sa machine à états détermine les actions à exécuter à chaque étape selon l'état courant, en suivant l'ordre strict *ProcessInput()*, *Update()*, puis *Render()*.

- ``GameManager`` rassemble les objets du jeu et constitue la source de vérité sur leur état.

Pour garantir un démarrage ordonné avant de traiter les entrées du joueur, nous avons également introduit les étapes *Initialisation()* et *Setup()*.


## 💻 Compilation
1. **cmake .** dans la console à la racine
2. Ouvrir Boba burst.sln
3. Clic droit Jolt project -> propreties
4. c/c++ -> Code Generation
5. Changer Runtime Library à /MDd
6. Compiler le projet au global

---

## ⌨️ Touches du clavier 
1. Flèche gauche -> déplacement vers la gauche
2. Flèche droite -> déplacement vers la droite
3. TAB -> changement de la caméra (1ère / 3ème personne)
4. ESC -> ouverture du menu
