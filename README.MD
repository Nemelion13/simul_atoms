# Simulation atomique 3D en C++ et Raylib

Ce projet affiche une représentation animée d'un atome : un noyau composé de
protons et de neutrons, des électrons qui tournent autour de lui, et une
interface pour choisir un élément chimique.

Le projet sert aussi de support d'apprentissage. Il montre comment séparer un
programme en plusieurs fichiers C++, comment représenter des données, comment
écrire des fonctions et comment traduire une règle scientifique en algorithme.
La configuration électronique est calculée à partir du numéro atomique : 

> **Important :** cette simulation est une illustration pédagogique. Les
> électrons réels ne suivent pas de petites trajectoires circulaires autour du
> noyau comme dans l'animation. Les orbites, les vitesses, les rayons et les
> inclinaisons sont des choix visuels, pas des mesures physiques.

## Sommaire

- [Ce que fait le programme](#ce-que-fait-le-programme)
- [Fichiers du projet](#fichiers-du-projet)
- [Installer et lancer le projet](#installer-et-lancer-le-projet)
- [Numéro atomique et électrons](#numéro-atomique-et-électrons)
- [Comprendre les configurations électroniques](#comprendre-les-configurations-électroniques)
- [Comment le code calcule une configuration](#comment-le-code-calcule-une-configuration)
- [Comment le code dessine les exposants](#comment-le-code-dessine-les-exposants)
- [Ce que l'algorithme ne modélise pas](#ce-que-lalgorithme-ne-modélise-pas)
- [Quelques notions de C++ utilisées](#quelques-notions-de-c-utilisées)
- [Exemples pour essayer](#exemples-pour-essayer)

## Ce que fait le programme

À son lancement, la fenêtre montre une simulation 3D. La liste défilante permet
de choisir l'un des 118 éléments chimiques définis dans `ATOMS` et d'accéder
aux éléments de tous les numéros atomiques, y compris les éléments lourds.

Pour l'élément sélectionné, le programme :

1. lit son numéro atomique et son nombre de neutrons ;
2. calcule sa configuration électronique ;
3. crée un électron animé pour chacun de ses électrons ;
4. dessine un noyau simplifié ;
5. affiche le nom de l'élément, ses nombres de protons, de neutrons et
   d'électrons, ainsi que sa configuration électronique.

L'animation des électrons se met à jour à chaque image. La configuration
chimique, elle, est recalculée à partir du numéro atomique de l'élément choisi.

## Fichiers du projet

| Fichier | Rôle |
|---|---|
| [`main.cpp`](./main.cpp) | Point d'entrée du programme, boucle d'animation, caméra, dessin de la scène et interface Raygui. |
| [`atoms.h`](./atoms.h) | Déclarations des structures et des fonctions liées aux atomes. |
| [`atoms.cpp`](./atoms.cpp) | Liste des éléments, ordre de remplissage des sous-couches et implémentation du calcul de configuration. |
| [`CMakeLists.txt`](./CMakeLists.txt) | Indique à CMake comment construire l'exécutable et où trouver Raylib et Raygui. |
| [`activate.ps1`](./activate.ps1) | Active le compilateur C++ et les outils MSYS2 dans un terminal PowerShell. |
| `.vscode/tasks.json` | Configure la tâche VS Code qui ouvre un terminal avec l'environnement du projet activé. |

La séparation entre `.h` et `.cpp` est une convention importante en C++ :

- un fichier **d'en-tête** (`.h`) explique aux autres fichiers quelles données
  et quelles fonctions sont disponibles ;
- un fichier **source** (`.cpp`) contient le corps des fonctions, c'est-à-dire
  les instructions exécutées.

Ainsi, `main.cpp` peut appeler `GetElectronicConfiguration` en incluant
`atoms.h`, sans contenir lui-même l'algorithme de chimie.

## Installer et lancer le projet

L'environnement configuré pour ce projet utilise :

- **C++17** et le compilateur GCC 16 ;
- **Raylib 6.2** pour la fenêtre, les graphismes 2D et la scène 3D ;
- **Raygui 5.0** pour la liste défilante des éléments ;
- **CMake** pour configurer la compilation ;
- **Ninja** comme outil de compilation ;
- **GDB** comme débogueur.

Raygui est une bibliothèque « header-only » : ses fonctions sont fournies par
un fichier d'en-tête plutôt que par une bibliothèque `.lib` ou `.a` séparée.
`raygui.h` doit être disponible dans le dossier d'inclusion UCRT64 de MSYS2.
Dans `main.cpp`, `RAYGUI_IMPLEMENTATION` est défini dans un seul fichier, avant
l'inclusion de `raygui.h`, pour y compiler l'implémentation de Raygui.

### Activer l'environnement

Ouvre un terminal PowerShell dans le dossier du projet et exécute :

```powershell
. .\activate.ps1
```

Le premier point suivi d'un espace est nécessaire : il exécute le script dans
le terminal actuel afin que ses variables d'environnement et son chemin soient
conservés après son exécution.

La tâche VS Code **Activer environnement C++ Raylib** effectue également cette
activation.

### Compiler et lancer

Depuis la racine du projet, dans le terminal activé :

```powershell
cmake -S . -B build -G Ninja
cmake --build build
.\build\simul_atoms.exe
```

- `cmake -S . -B build -G Ninja` configure le projet. `-S .` désigne le
  dossier des sources, et `-B build` le dossier où ranger les fichiers générés.
- `cmake --build build` compile le programme.
- `.\build\simul_atoms.exe` lance la simulation.

Après une modification du code, il suffit généralement de relancer :

```powershell
cmake --build build
.\build\simul_atoms.exe
```

### Lancer l'application depuis le Bureau

Sous Windows, le projet copie les DLL nécessaires à côté de l'exécutable lors
de la compilation. Après avoir exécuté `cmake --build build`, tu peux lancer
`build\simul_atoms.exe` directement, sans ouvrir VS Code ni activer
l'environnement dans PowerShell. Le dossier `build\Tools` contient également
le logo et les fichiers audio utilisés par l'application.

Pour créer un raccourci Bureau, fais un clic droit sur `build\simul_atoms.exe`,
puis **Afficher d'autres options > Envoyer vers > Bureau (créer un raccourci)**.
Le raccourci ouvre la même application ; il ne démarre pas automatiquement avec
Windows.

Le dossier `build` contient les fichiers générés par CMake ; ce ne sont pas
les fichiers source du programme.

## Numéro atomique et électrons

Le **numéro atomique**, noté \(Z\), est le nombre de protons du noyau. Il
permet d'identifier l'élément :

- l'hydrogène a \(Z = 1\) ;
- le lithium a \(Z = 3\) ;
- le carbone a \(Z = 6\) ;
- l'oxygène a \(Z = 8\).

Un atome neutre possède autant d'électrons que de protons. Il possède donc
\(Z\) électrons. Dans ce programme, chaque entrée de `ATOMS` contient
`atomicNumber`, qui représente le nombre de protons et sert aussi de nombre
d'électrons pour l'atome neutre.

Le nombre de neutrons est une autre information : il n'est pas déduit du
numéro atomique. Par exemple, un même élément peut exister sous plusieurs
formes, appelées **isotopes**, ayant des nombres de neutrons différents.
Pour remplir la liste, les éléments naturels utilisent le nombre de neutrons
de leur isotope de plus grande abondance naturelle, d'après le tableau CIAAW
2024 des compositions isotopiques. Les éléments sans isotope naturellement
abondant utilisent l'isotope de référence dont le nombre de masse est indiqué
entre crochets dans les tableaux périodiques. Cette valeur est un choix
représentatif pour la visualisation, et non une affirmation que tous les
atomes de cet élément ont ce nombre de neutrons.

Le nombre de masse \(A\) est la somme des protons et des neutrons. Dans la
liste, on stocke directement le nombre de neutrons ; il est calculé par
\(N = A - Z\), où \(Z\) est le numéro atomique. Par exemple, le carbone
représenté est le carbone 12 : \(Z=6\), \(A=12\), donc \(N=6\).

Source des compositions isotopiques : [CIAAW, Isotopic Compositions of the
Elements 2024](https://ciaaw.org/isotopic-abundances.htm). Pour les isotopes
de référence des éléments sans abondance naturelle, voir le
[tableau périodique publié par Bowserinator](https://github.com/Bowserinator/Periodic-Table-JSON).

### Données d'un atome en C++

Dans `atoms.h`, on trouve :

```cpp
struct AtomConfig {
    const char* name;
    int atomicNumber;
    int neutrons;
};
```

Une `struct` regroupe des valeurs qui décrivent un même objet. Un `AtomConfig`
regroupe ainsi le nom de l'élément, son numéro atomique et le nombre de
neutrons choisi pour la simulation.

La liste `ATOMS`, dans `atoms.cpp`, est un `std::vector<AtomConfig>` : un
conteneur dynamique qui conserve plusieurs configurations d'atomes.

```cpp
{ "Carbone (C)", 6, 6 }
```

Cette ligne crée un élément de la liste. Ses trois valeurs correspondent,
dans l'ordre, aux trois champs de `AtomConfig`.

## Comprendre les configurations électroniques

Les électrons ne sont pas tous interchangeables : dans la description
chimique, ils occupent des **couches** et des **sous-couches électroniques**.
Une notation telle que `2p^4` signifie :

- `2` : la couche principale, définie par le nombre quantique principal \(n\) ;
- `p` : le type de sous-couche ;
- `4` : le nombre d'électrons présents dans cette sous-couche.

Les lettres des sous-couches sont, par convention, **minuscules** : `s`, `p`,
`d` et `f`. L'expression « couche 2 » n'est donc pas la même chose que la
sous-couche `2s` : la couche 2 peut contenir à la fois `2s` et `2p`.

### Capacité des sous-couches

Les sous-couches peuvent accueillir au maximum :

| Sous-couche | Nombre d'orbitales | Nombre maximal d'électrons |
|---|---:|---:|
| `s` | 1 | 2 |
| `p` | 3 | 6 |
| `d` | 5 | 10 |
| `f` | 7 | 14 |

Chaque orbitale peut accueillir au maximum deux électrons, de spins opposés.
La capacité d'une sous-couche est donc deux fois son nombre d'orbitales.
En mécanique quantique, les nombres d'orbitales `1`, `3`, `5` et `7`
correspondent respectivement aux valeurs \(2l+1\) pour `s`, `p`, `d` et `f`.

### Ordre de remplissage : règle de Klechkowski

Pour construire la configuration fondamentale usuelle, on remplit les
sous-couches dans un ordre énergétique précis. L'ordre est parfois appelé
règle de **Klechkowski**, ou règle de **Madelung**. Il ne consiste pas
simplement à remplir entièrement une couche avant de passer à la suivante :
les sous-couches de différentes couches s'entremêlent.

L'ordre programmé dans `ORBITALS` est :

```text
1s → 2s → 2p → 3s → 3p → 4s → 3d → 4p → 5s → 4d
   → 5p → 6s → 4f → 5d → 6p → 7s → 5f → 6d → 7p
```

Cela explique, par exemple, pourquoi `4s` est rempli avant `3d` dans l'ordre
de base suivi par le programme.

Cette règle est liée à la tendance énergétique empirique \(n+l\) :

- on remplit généralement en premier la sous-couche dont \(n+l\) est le plus
  petit ;
- si deux sous-couches ont la même valeur de \(n+l\), celle qui a le plus
  petit \(n\) est généralement remplie en premier.

Cette règle donne un ordre de départ pratique, mais elle ne détermine pas
parfaitement toutes les configurations observées : dans les atomes réels, les
énergies sont proches et peuvent varier avec l'occupation des sous-couches.

### Deux autres règles utiles

Une configuration électronique complète est aussi soumise à deux principes
importants :

1. **Principe d'exclusion de Pauli** : deux électrons d'un même atome ne
   peuvent pas avoir exactement les quatre mêmes nombres quantiques. Une
   orbitale ne contient donc pas plus de deux électrons.
2. **Règle de Hund** : dans des orbitales de même énergie d'une sous-couche,
   les électrons occupent d'abord des orbitales différentes avec des spins
   parallèles avant de s'apparier.

Le programme respecte les **capacités totales** `s`, `p`, `d` et `f`, mais
il ne représente pas les orbitales individuelles ni les spins. Il ne simule
donc pas en détail les règles de Pauli et de Hund : elles sont expliquées ici
pour replacer le calcul dans son contexte chimique.

### Exemples de configurations

Dans le texte, le symbole `^` introduit l'exposant. L'interface graphique
dessine, elle, le nombre de façon plus petite et légèrement surélevée :

| Élément | \(Z\) | Configuration écrite |
|---|---:|---|
| Hydrogène | 1 | `1s^1` |
| Lithium | 3 | `1s^2 2s^1` |
| Carbone | 6 | `1s^2 2s^2 2p^2` |
| Oxygène | 8 | `1s^2 2s^2 2p^4` |
| Chrome | 24 | `1s^2 2s^2 2p^6 3s^2 3p^6 4s^1 3d^5` |
| Cuivre | 29 | `1s^2 2s^2 2p^6 3s^2 3p^6 4s^1 3d^10` |

Le chrome et le cuivre illustrent une limite de l'ordre de remplissage simple.
Pour ces éléments, le transfert d'un électron de `4s` vers `3d` donne la
configuration représentée ici.

## Comment le code calcule une configuration

Les déclarations des types et fonctions se trouvent dans
[`atoms.h`](./atoms.h). Les implémentations se trouvent dans
[`atoms.cpp`](./atoms.cpp).

### Décrire une sous-couche

```cpp
struct ElectronicSubshell {
    int principalNumber;
    char orbital;
    int electronCount;
};
```

Un `ElectronicSubshell` décrit exactement un terme comme `2p^4` :

- `principalNumber` vaut `2` ;
- `orbital` vaut `'p'` ;
- `electronCount` vaut `4`.

Le programme représente une configuration complète comme
`std::vector<ElectronicSubshell>`, autrement dit une suite de ces termes.

### La table `ORBITALS`

La structure interne `Orbital` contient le nombre principal, la lettre et
la capacité maximale d'une sous-couche. Le tableau `ORBITALS` contient les
19 sous-couches dans l'ordre de remplissage ci-dessus.

```cpp
{ 2, 'p', 6 }
```

Cette entrée signifie que `2p` peut contenir jusqu'à six électrons. Le tableau
est un `std::array`, c'est-à-dire un tableau de taille fixe connue à la
compilation. Le mot-clé `constexpr` indique au compilateur que son contenu
est une constante connue à la compilation.

### `GetElectronicConfiguration`

La fonction est déclarée ainsi :

```cpp
std::vector<ElectronicSubshell> GetElectronicConfiguration(int atomicNumber);
```

On peut la lire de droite à gauche : elle reçoit un `int` appelé
`atomicNumber` et renvoie un `std::vector<ElectronicSubshell>`.

Son travail se déroule en plusieurs étapes :

1. **Vérifier l'entrée.** Le nombre atomique doit être compris entre 1 et
   118. Sinon, le code lance une `std::out_of_range`. Il évite ainsi de
   fabriquer silencieusement une configuration impossible pour cette version
   du programme.
2. **Compter les électrons à placer.** Pour un atome neutre, la valeur de
   départ est le numéro atomique : `int remainingElectrons = atomicNumber;`.
3. **Parcourir les sous-couches dans l'ordre.** Une boucle `for` visite
   chaque entrée de `ORBITALS`.
4. **Remplir sans dépasser la capacité.** La fonction
   `std::min(remainingElectrons, orbital.capacity)` choisit le plus petit de
   ces deux nombres. La sous-couche ne reçoit donc jamais plus que sa
   capacité, et on soustrait ensuite les électrons placés au total restant.
5. **Corriger certaines configurations connues.** Une table dédiée décrit
   quelques exceptions observées ; `TransferElectrons` déplace les électrons
   indiqués d'une sous-couche vers une autre.
6. **Supprimer les termes vides.** Une exception peut vider une sous-couche.
   `std::remove_if`, associé à `erase`, retire de la liste les termes dont le
   nombre d'électrons est nul.
7. **Renvoyer le résultat.** Le `return configuration;` transmet la liste
   calculée à l'appelant.

Par exemple, pour le carbone (\(Z=6\)) :

```text
Départ : 6 électrons
1s reçoit 2 → il en reste 4
2s reçoit 2 → il en reste 2
2p reçoit 2 → il en reste 0
Résultat : 1s^2 2s^2 2p^2
```

### Comment les exceptions sont décrites

`ConfigurationException` indique une sous-couche donneuse, une sous-couche
receveuse, et le nombre d'électrons à déplacer. Le tableau
`CONFIGURATION_EXCEPTIONS` associe ces transferts à un numéro atomique précis.

La fonction `TransferElectrons` parcourt la configuration avec une boucle sur
des références modifiables :

```cpp
for (ElectronicSubshell& subshell : configuration)
```

Une référence (`&`) désigne ici directement un élément du vecteur. Modifier
`subshell.electronCount` modifie donc l'élément contenu dans `configuration`,
et non une copie.

La fonction cherche les sous-couches qui correspondent au couple
`(principalNumber, orbital)`, retire l'effectif de la sous-couche donneuse et
l'ajoute à la sous-couche receveuse. L'effectif total d'électrons est ainsi
conservé.

### `FormatElectronicConfiguration`

```cpp
std::string FormatElectronicConfiguration(
    const std::vector<ElectronicSubshell>& configuration
);
```

Cette fonction convertit la liste en texte lisible, par exemple :

```text
1s^2 2s^2 2p^2
```

Le caractère `^` est une convention typographique en texte brut : il signifie
que le nombre qui suit est un exposant. `std::to_string` convertit les nombres
entiers en chaînes, et le programme assemble ces morceaux dans un
`std::string`.

La notation textuelle et le dessin graphique sont deux opérations différentes.
La fonction `FormatElectronicConfiguration` prépare une chaîne ; elle n'est
pas nécessaire pour dessiner l'interface actuelle.

### `CreateElectrons`

`CreateElectrons(AtomConfig config)` s'appuie sur le résultat de
`GetElectronicConfiguration` pour créer les électrons de l'animation :

1. la configuration est calculée ;
2. les effectifs `1s`, `2s`, `2p`, etc. sont additionnés par couche
   principale `n` dans le tableau `electronsPerShell` ;
3. une boucle crée autant d'objets `Electron` que cette couche contient
   d'électrons.

Par exemple, le carbone a la configuration `1s^2 2s^2 2p^2`. La couche 1
contient donc deux électrons, et la couche 2 en contient quatre. La simulation
crée au total six électrons animés.

Chaque `Electron` contient un rayon, une vitesse angulaire, un angle initial,
une inclinaison, une couleur et une file `trail` de positions précédentes.
La file est un `std::deque<Vector3>`, un conteneur pratique pour ajouter et
retirer des positions à ses deux extrémités. La boucle principale de
[`main.cpp`](./main.cpp) actualise ces valeurs et dessine la trajectoire.

### `DrawElectronicConfiguration`

Cette fonction, définie dans `main.cpp`, reçoit la liste de sous-couches
calculée et dessine chaque terme dans l'interface :

1. elle fabrique le texte de base, par exemple `2p`, et le texte de l'exposant,
   par exemple `4` ;
2. `MeasureText` estime la largeur du terme pour placer le nombre juste
   après ;
3. `DrawText` dessine le terme à la taille normale ;
4. `DrawText` dessine le nombre plus petit et quelques pixels plus haut.

La configuration est donc construite comme des données structurées avant
d'être transformée en texte ou dessinée. C'est plus facile à faire évoluer que
de construire une seule grande chaîne pour essayer d'en calculer les
exposants.

## Ce que l'algorithme ne modélise pas

Le calcul proposé est utile pour apprendre l'ordre de remplissage et les
conteneurs C++, mais il n'est pas un calculateur quantique complet :

- l'ordre de remplissage est une approximation pratique des configurations
  fondamentales, et les exceptions sont explicitement limitées à celles
  présentes dans le tableau `CONFIGURATION_EXCEPTIONS` ;
- toutes les subtilités expérimentales des éléments lourds ne sont pas
  encodées. Pour certaines configurations, les valeurs admises peuvent varier
  selon le niveau de référence et la source scientifique ;
- la règle de Hund et la projection des électrons sur des orbitales
  individuelles ne sont pas calculées ;
- les ions ne sont pas pris en charge : le programme suppose que l'atome est
  neutre et que son nombre d'électrons vaut \(Z\) ;
- les nombres de neutrons des éléments du menu sont saisis dans `ATOMS` ;
- `CreateElectrons` regroupe les électrons par couche principale pour les
  besoins de l'animation. Il ne montre ni les formes réelles des orbitales
  atomiques ni leurs probabilités de présence ;
- les rayons, les vitesses, les inclinaisons, les traînées et la forme du
  noyau sont arbitraires. Ils servent à rendre la scène compréhensible.

En particulier, l'animation n'affirme pas que tous les électrons d'une couche
suivent la même trajectoire dans un véritable atome.

## Quelques notions de C++ utilisées

### Déclaration et définition

Dans `atoms.h`, écrire :

```cpp
std::vector<ElectronicSubshell> GetElectronicConfiguration(int atomicNumber);
```

annonce seulement l'existence de la fonction et son interface. C'est sa
**déclaration**. La fonction complète, avec son corps entre accolades, est
définie dans `atoms.cpp`. CMake compile `main.cpp` et `atoms.cpp`, puis lie
leurs fichiers objets dans un seul exécutable.

### Paramètres, résultat et `return`

Dans :

```cpp
std::vector<Electron> CreateElectrons(AtomConfig config)
```

- `AtomConfig config` est le paramètre fourni à la fonction ;
- `std::vector<Electron>` est le type du résultat ;
- `return electrons;` renvoie le vecteur construit.

Pour une fonction sans résultat, C++ utilise souvent le type `void`. Ici, la
fonction doit fournir sa liste d'électrons ; elle a donc un type de retour
`std::vector<Electron>`.

### `std::vector` et `std::array`

- `std::vector<T>` contient une liste de valeurs de type `T` dont la taille
  peut évoluer ;
- `std::array<T, N>` contient exactement `N` valeurs et sa taille est fixe.

La configuration finale et la liste d'électrons sont des vecteurs, car leur
contenu dépend de l'élément. `ORBITALS` utilise un tableau fixe, car son ordre
et son nombre d'entrées sont définis par le programme.

### Constantes et références en lecture seule

`const` interdit de modifier une valeur par cette variable ou référence. Dans :

```cpp
for (const ElectronicSubshell& subshell : configuration)
```

la référence évite de copier chaque élément du vecteur, et `const` garantit
que cette boucle ne le modifie pas.

Au contraire, `ElectronicSubshell& subshell` dans `TransferElectrons` autorise
la modification des valeurs existantes.

### Boucles

Une boucle `for` répète des instructions. Les boucles de ce projet servent,
par exemple, à :

- parcourir les orbitales dans l'ordre ;
- parcourir les exceptions possibles ;
- créer un électron pour chaque électron d'une couche ;
- parcourir les éléments du menu.

### Espace de noms anonyme

Dans `atoms.cpp`, un bloc :

```cpp
namespace {
    // détails privés à atoms.cpp
}
```

rend les noms déclarés à l'intérieur privés à ce fichier source. `Orbital`,
`ConfigurationException`, `ORBITALS`, `CONFIGURATION_EXCEPTIONS` et
`TransferElectrons` sont des détails internes de l'algorithme : `main.cpp`
n'a pas besoin de les connaître.

### Gardes d'inclusion

`atoms.h` utilise :

```cpp
#ifndef ATOMS_H
#define ATOMS_H
// contenu du fichier
#endif
```

Ce mécanisme empêche qu'un même fichier d'en-tête soit inclus plusieurs fois
au cours d'une compilation. Sans cette protection, le compilateur pourrait
rencontrer plusieurs définitions identiques.

### Bibliothèques standard et Raylib

- `<vector>`, `<array>`, `<string>`, `<algorithm>` et `<stdexcept>` font partie
  de la bibliothèque standard C++ ;
- `raylib.h` fournit entre autres `Color`, `Vector3`, `DrawText` et les
  fonctions graphiques ;
- `raygui.h` fournit le menu et doit être inclus avec son implémentation dans
  un seul fichier source.

## Exemples pour essayer

Après avoir inclus `atoms.h`, le programme peut demander une configuration :

```cpp
std::vector<ElectronicSubshell> carbone =
    GetElectronicConfiguration(6);
```

Ici, `6` est le numéro atomique du carbone. Pour obtenir sa représentation
textuelle :

```cpp
std::string texte = FormatElectronicConfiguration(carbone);
```

La chaîne obtenue est :

```text
1s^2 2s^2 2p^2
```

Pour afficher cette chaîne dans une application console, ajouter
`#include <iostream>` puis écrire `std::cout << texte << '\n';`. Dans
l'application graphique actuelle, c'est `DrawElectronicConfiguration` qui
dessine les termes et les exposants à l'écran.

### Resources externes
  - logo.png : capture d'écran de l'interface
  - generic.mp3 : (youtube) INFINITY - Epic Futuristic Music Mix |       Atmospheric Sci-Fi Music 
  - btn_beat.mp3 : https://pixabay.com/fr/users/juniorsoundays-19205462/