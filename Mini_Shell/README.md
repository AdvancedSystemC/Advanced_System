Benoit DUVERNAY & Alexandre GERBERON

# Projet MiniShell

## How-to-use

```
make
./bin/my_sh
```

## Commandes

- [x] cd
  
  ```
  Usage : 
  cd [PATH]
  ```

- [x] pwd
  
  ```
  Usage :
  pwd
  ```

- [x] echo
  
  ```
  Usage : 
  echo [input]
  ```

- [x] ps
  
  ```
  Usage :
  ps
  ```

- [x] ls
  
  ```
  Usage : 
  ls [OPTION]... [FILE/FOLDER]...
      -a do not ignore entries starting with .
      -i print the index number of each file
      -l use a long listing format
      -R list subdirectories recursively
  ```

- [x] history
  
  ```
  Usage : 
  history
  ```

- [x] exit
  
  ```
  Usage :
  exit
  ```

- [x] who
  
  ```
  Usage : 
  who
  ```

- [x] alias
  
  ```
  Usage : 
  alias
  ```

## Logique implémentée / choix de conception

#### Input parser

Implémente un analyseur d'entrée.
Il prend une chaîne de caractères (l'entrée utilisateur) et la divise en une structure de commande, avec la prise en charge des redirections d'entrée/sortie et des arguments.


1. Analyse de la commande et des arguments : divise la chaîne d'entrée en tokens

2. Gestion des redirections : gère les redirections d'entrée (`<`) et de sortie (`>` et `>>`)

3. Construction de la structure de commande : Crée une structure Command contenant le nom de la commande, les arguments et les informations de redirection

4. Gestion de l'exécution en arrière-plan

#### Command manager

Est implémenté sous la forme d'un référentiel de commande à travers un tableau [nom de la fonction][signature de la fonction]

Implémente la gestion des commandes et de leur exécution : 

- Avec ou sans redirection d'entrée/sortie.

- Gestion des tubes (non fonctionnel)



## Temps de travail

| Nom       | Tâche                               | Terminé | Durée |
| --------- | ----------------------------------- | ------- | ----- |
| Alexandre | implémentation de ls                | ✅       | 5h00  |
|           | implémentation de pwd               | ✅       | 0h15  |
|           | implémentation de who               | ✅       | 0h15  |
|           | implémentation de cd                | ✅       | 0h15  |
|           | implémentation de echo              | ✅       | 0h15  |
|           | implémentation de grep / pipe       | ❌       | 1h00  |
| Benoit    | implémentation alias                | ⚠️      | 1h00  |
|           | implémentation history              | ✅       | 1h00  |
|           | implémentation ps                   | ✅       | 0h30  |
|           | implémentation exit                 | ✅       | 0h02  |
|           | implémentation display/ihm          | ✅       | 0h30  |
|           | implémentation << >> < >            | ✅       | 3h00  |
|           | implémentation référentiel commande | ✅       | 1h00  |
|           | implémentation du parser            | ✅       | N/A   |
|           | Refacto                             | ✅       | 3h00  |
|           | Implémentation pipe et && \||       | ❌       | 4h00  |
| Total     |                                     |         | 21h02 |

## Difficultés rencontrées

- Difficultés liées au langage de programmation C : nous pensons que le sujet aurait été beaucoup plus simple avec un langage orienté objet, notamment sur la gestion des pipelines. Nous avons encore quelques difficultés concernant les pointeurs et les allocations mémoires en C.

- Architecture : Nous nous sommes hâtés sur une solution, et cela nous a contraints à effectuer plusieurs refactorisations. Il aurait fallu une dernière pour réussir à gérer les pipelines de manière convenable.

- Temps de travail : sujet très chronophage
