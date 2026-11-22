# Boba-burst

> **Un projet captivant développé avec passion et un travail d'équipe hors pair !**

---

##  À propos du projet

**Buba-burst** est un jeu dynamique conçu pour offrir une expérience fluide, amusant et engageante. Ce projet démontre notre capacité à collaborer efficacement, à concevoir des fonctionnalités robustes et à offrir une interface propre et réactive dans le contexte du programme DDJV de l'université de Sherbrooke.

---

## Technologies & Outils

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

Dans notre façon d'implémenter on a pas mal respecté notre graphique de notre GDD. Par contre, nous avons pas mal rajouté d'élément, car le projet a fallu plus d'élément que prévu pour réaliser notre vision.On a ajouté une classe LevelLoader qui load l'emplacement de tout les objet grace a un fichier JSON ce qui permet de bouger les objets sans avoir a recompilé tout.On a ajouté toute les classes qui sont nécessaire a un rendu Vulkan : VulkanContext, VulkanRenderer et Scene. On a aussi des classes pour le rendu 2D pour le UI : le UIRenderer. On a aussi la classe qui fait le lien avec Jolt: PhysicWorld. On a rajouté aussi un gestionnaire d'event: EventBus qui utilise le pattern de subscribers pour envoyer des messages a tous ceux qui veulent recevoir un certain type d'event tout les event hérite de Event qui est plus simplifier que ce qu'on a écrit dans le GDD. On a aussi ajouté des classification de plus comme Goal qui hérite de track pour que son collider sache qu'on terminer le jeu. On a enlevé Face, car sa ne marchait pas bien avec le système qu'on faisait comme nous on expliqué les professeurs.

J'ai du beaucoup modifier notre boucle de jeu comparé au GDD. Une fois que les choses ce sont concrétisé j'ai pus voir comment les choses vont s'exécuter. On a une classe Game qui est un singleton. et une classe GameManager qui est aussi un singleton. La classe Game est l'application dans lequel tout est appelé pour l'éxécution du programme. Le GameManager est l'objet qui regroupe tout les objets en un endroit pour les appeler quand on a besoin d'eux. On a aussi le GameState qui est un membre de Game qui fait office de machine d'état gameloop comme décrit dans le GDD. les fonctions : ProcessInput(), Update(), Render() de la classe Game éxécute leurs fonctionalité dépendant de la valeur de la variable gameState. On a aussi rajouté la fonction Initialisation() et Setup() dans Game, la première pour initialiser les bibliothèque et valeurs nécessaire au fonctionnement de l'application et la seconde pour assigner les valeurs de départ de notre jeu au objet du jeu.

---

## Compiling
1. cmake . in cmd at root
2. open Boba burst.sln
3. right click Jolt project -> propreties
4. c/c++ -> Code Generation
5. change Runtime Library to /MDd
6. compile whole project
