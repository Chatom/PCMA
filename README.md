PCMA
====

Pascal Compiler for Mips Architecture

https://github.com/Chatom/PCMA

PCMA est un compilateur pascal vers MIPS écrit en C.


Il est décomposé en plusieurs parties :
- analyse lexicale (caractère par caractère)
- analyse syntaxique (selon la grammaire) et génération d'un arbre syntaxique
- parcours de cet arbre et analyse sémantique
	(génération d'une table des symboles et vérification des types...)
- génération de code MIPS durant ce parcourt


->  How To :

Compilation : taper "make" dans le répertoire dirfile
Execution : ./PCMA.run FichierPascal [verbose] [tree && symb]

Les deux options entre [] acceptent n'importe quels caractères.
[verbose] affiche l'analyse lexicale de manière plus détaillée.
[tree && symb] affiche l'arbre syntaxique et la table des symboles.

Des messages d'erreur lors de l'analyse sémantique peuvent être affichés, mais n'empêcheront pas la compilation.
! Le fichier MIPS est automatiquement créé dans "out.asm".


->  Choix :

L'analyse lexicale suit les lexèmes autorisés dans la grammaire.
De même, l'analyse syntaxique suit la grammaire simplifiée Pascal donnée.
Ce parcours syntaxique est effectuée avec une fonction spécialisée à chaque fois.

Durant ce parcours, un arbre est créé (il est composé de structures noeuds comportant des fils (unions) et un type.

Cet arbre, une fois généré, sera alors parcouru à la fois pour son affichage et pour la génération de la table des symboles et du MIPS.
Ce parcours est récursif : la fonction est appelée récursivement et effectue un parcours du noeud différent selon son type.

La table des symboles est créée principalement à l'aide de morceaux itératifs au milieu de ce parcours (déclarations de variables et fonctions).
Les vérifications de type sont effectuées en recherchant les variables dans la table des symboles si c'est une variable.
J'utilise également des variables globales, selon les types à comparer.

Le code MIPS est généré en même temps que l'analyse sémantique.
Le MIPS gère actuellement les affectations, opérations, if et while, et les read/write.

Les fonctions génèrent actuellement un jal, jr $ra... Mais leur usage entraîne des soucis car ceci n'a pas encore été implémenté.

Beaucoup d'affichage sont en commentaire dans le fichier TableSymbole.c et permettent de débugger.


Le code étant libre et disponible sur github, il sera probablement soumis à de futures modifications et améliorations afin de gérer bien plus de cas.
