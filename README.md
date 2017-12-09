# PROJET DE COMPILATION

## Description

Ce projet est réalisé dans le cadre de l'UE "Compilation".
L'objectif est de réaliser une traduction depuis le code de haut niveau de StenC vers un code assembleur MIPS.

Les outils utilisés sont :
- (f)lex : pour générer l'analyseeur lexical à partir de la specification des tokens
- Yacc : pour générer l'analyseur syntaxique à partir de la specification de grammaire
- R2000 SPIM : pour simuler l'exécution du code assembleur MIPS généré

Auteurs :
- BANZAN-WONG Juan-Andres (Dawguie)
- PERRIN Romain (Romain96)

## StenC

le StenC [Sten-Ci] est un sous ensemble du C auquel on a ajouté un type stencil et un opérateur d'application de stencil.

- le seul type de base est "int" (et ses tableaux multidimensionnels)
- les structures de contrôle if, else, for et while
- les opérateurs +, -, ++, --, /, *
- le nouveau type stencil et son opérateur d'application "$"
- toutes les variables sont locales et statiques
- les fonctions peuvent être supportées et peuvent être récursives

## Milestones [en construction]

|    DATE    |    DESCRIPTION    |
|------------|-------------------|
| 2017-11-05 | Début du projet   |
| 2017-11-19 | Première ébauche d'une grammaire reconnaissant 75-80% du StenC |
| 2017-11-26 | Génération de code MIPS pour les expressions arithmétiques |
| 2017-11-27 | Génération de listes d'expressions arithmétiques + affectations (entier) |
| 2017-11-29 | Génération du code MIPS des fonctions printf et printi |
| 2017-11-30 | Génération de code MIPS pour les conditions booléennes |
| 2017-12-02 | Génération de code MIPS pour les structures de contrôle (if, else, for, while) |
| 2017-12-09 | Génération de code MIPS pour les tableaux et références de tableaux |
|     ?      |        ?          |

## Spécification complète du compilateur [à venir]

Cette section décrit la spécification complète de nôtre compilateur c'est-à-dire tout ce qu'il est capable de 
reconnaître et de produire en terme de code assembleur.
