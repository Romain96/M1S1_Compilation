# Compilateur StenC vers MIPS32

## Description

Objectif : L'objectif de ce projet univeritaire est de réaliser un compilateur capable de traduire un code écrit dans le langage StenC en un code assembleur écrit pour une architecture MIPS32 bits pouvant être exécuté par un simulateur comme SPIM ou MARS.

Organisation :
- Encadrement : Prof. Cédric BASTOUL et Philippe CLAUSS
- UE : Compilation
- Niveau : Master I
- Cursus : Informatique et Sciences de l'Image (ISI)
- Session : semestre I automne 2017
- Etablissement : Université de Strasbourg

Outils :
- (f)lex : générateur d'analyseur lexical
- Yacc : générateur d'analyseur syntaxique
- R2000 SPIM : simulateur de programmes MIPS32
- 
Auteurs :
- BANZAN-WONG Juan-Andres
- PERRIN Romain

## Le langage StenC

le StenC [Sten-Ci] est un sous ensemble du C auquel on a ajouté un type stencil et un opérateur d'application de stencil.

- il n'existe qu'un seul type de base du C : **int**
- il est possible de définir des **tableaux d'entiers**
- les structures de contrôle acceptées sont : **if**, **else**, **for**, **while**
- les opérateurs acceptés sont : **+**, **-**, **\***, **/**, **++**, **- -**
- les variables sont toutes **locales** et **statiques**
- il est possible de définir des **fonctions** éventuellement *récursives*
- il existe un nouveau type **stencil**
- il existe un nouvel opérateur **$** d'application des stencils

## Milestones [en construction]

|    DATE    |    DESCRIPTION    |
|------------|-------------------|
| 2017-11-05 | Début du projet   |
| 2017-11-19 | Première ébauche d'une grammaire reconnaissant le StenC |
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
