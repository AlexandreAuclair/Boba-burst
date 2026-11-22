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

En général, nous avons bien respecté ce que nous avions établi comme base dans le GDD. Cependant, comme dans tout projet, nous avons dû effectuer quelque changement après avoir réalisé les premières implémentations.

Premièrement, nous avons ajouté la classe « LevelLoader » qui, au démarrage du jeu, nous permet d’instancier en mémoire les divers éléments d’un niveau précis. En effet, en bâtissant un fichier de type « .JSON » pour le « LevelLoader » contenant les éléments de notre scène, nous pouvons effectuer des modifications des objets du niveau sans avoir a effectué une nouvelle compilation du projet.

Par la suite, nous avons aussi ajouté toutes les classes nécessaires afin d’obtenir notre rendu Vulkan. C’est-à-dire, « VulkanContext », « VulkanRenderer » et « Scene ». De plus, afin de rendre notre menu 2D, nous lui avons créer une classe séparer de notre rendu 3D mentionner plutôt. Nous l’avons appelé « UIRenderer ».

En continuant, nous avons modifié nos liens avec Jolt ainsi que nos événements. Dans cette version du projet, tous nos appels à Jolt sont effectués dans une classe appelée « PhysicWorld ». Tout comme avec notre rendu, cela nous permet d’isoler Jolt dans une partie centraliser du projet afin de permettre un meilleur contrôle sur la physique ainsi que des améliorations futures simplifier. Du côté de nos événements, nous avons réussi à grandement réduire le nombre d’événements différents grâce à l’implémenter de la classe « EventBus ». Il s’agit d’un gestionnaire d’événement unique qui permet à tous ceux qui le veulent de s’inscrire afin de recevoir des messages d’un certain type ou d’en émettre avec le schéma de conception « Publisher-Subscriber ». Pour nos messages / « Événement », ils héritent tous d’une classe commune et peuvent passer des arguments afin de réduire grandement le nombre de classes d’événement nécessaire.

Pour l’architecture des niveaux, nous avons ajouté une spécification pour les buts afin qu’un détecteur puisse noter la fin du jeu et nous avons retiré le concept de face comme il ajoutait beaucoup trop de complexité pour le projet selon les commentaires des enseignants.

Pour terminer, nous avons grandement modifié notre boucle de jeu. Nos classes « Game » et « GameManager » sont des singletons permettant un contrôle centralisé de la boucle de jeux. « Game » est la classe principale du jeu contenant toutes les fonctions nécessaires appelé pour l’exécution du programme tandis que « GameManager » regroupe tous les objets au même endroit et contrôle leur vérité. Dans « Game » , notre machine dicte quelles fonctions sont exécutées. Ses dernières sont « ProcessInput(), Update(), Render() » dans cet ordre avec chacune d’entre elles regardant l’état courant pour décider des fonctions à exécuter. Afin de garder l’ordre, nous avons aussi ajouté les fonctions « Initialisation() » et « Setup() » pour établir les bases du jeu avant d’exécuter la volonté du joueur.


---

## Compiling
1. cmake . in cmd at root
2. open Boba burst.sln
3. right click Jolt project -> propreties
4. c/c++ -> Code Generation
5. change Runtime Library to /MDd
6. compile whole project

---

## LES TOUCHES
1. flèche gauche du clavier -> déplacement vers la gauche
2. flèche droite du clavier -> déplacement vers la droite
3. TAB -> changement de la caméra
4. ESC -> ouverture du menu
