# PROJET DE COMPILATION

## Description

Ce projet est réalisé dans le cadre de l'UE "Compilation".
L'objectif est de réaliser une traduction depuis le code de haut niveau de StenC vers un code assembleur MIPS.

Les outils utilisés sont :
- (f)lex pour générer le programme d'analyse lexicale
- Yacc pour générer le programme d'analyse syntaxique
- R2000 SPIM : pour simuler l'exécution du code assembleur MIPS généré

Auteurs :
- BANZAN-WONG Juan-Andres (Dawguie)
- PERRIN Romain (Romain96)

## StenC

le StenC [Sten-Ci] est un sous ensemble du C auquel on a ajouté un type stencil et un opérateur d'application de stencil.

- le seul type de base est "int" (et ses tableaux multidimensionnels)
- les structures de contrôle if, then, else, for et while
- les opérateurs +, -, ++, --, /, *
- le nouveau type stencil et son opérateur d'application "$"
- toutes les variables sont locales et statique
- les fonctions peuvent être supportées et peuvent être récursives

## Milestones

en construction
