# prediction-medicale
Prédiction des hospitalisations futures de patients atteints d'une pathologie donnée.

# Contexte

Cette application a été créée dans le cadre d'un travail de recherche au LIRMM, au sein de l'équipe ADVANSE, en collaboration avec le CHU de Nîmes.

### Collaborateurs

* Jessica Pinaire jessica.pinaire_at_chu-nimes.fr
* Jérôme Azé jerome.aze_at_lirmm.fr
* Sandra Bringay sandra.bringay_at_lirmm.fr

### Contribution

Copyright (c) 2016 Stella Zevio.

# Principe

L'application permet de déterminer les hospitalisations futures de patients atteints d'une pathologie donnée.

### Motif d'un patient

Un motif est associé à un patient. Il s'agit de la liste des hospitalisations qu'il a subies. 
Les motifs sont représentés par une suite d'entiers. Chaque entier correspond à une hospitalisation.

```
1 2
```

Ici, le patient a déjà subi deux hospitalisations. On cherche à déterminer, s'il était à nouveau hospitalisé, quelle serait la nature de cette nouvelle hospitalisation.

Lorsqu'un patient possède un motif de plus de 3 hospitalisations, nous décidons de ne baser la prédiction que sur les trois hospitalisations les plus récentes.

Ainsi, pour le motif suivant : 

```
1 2 3 4
```

La prédiction ne portera que sur le motif :

```
2 3 4
```

### Contexte médical

La prédiction ne peut être réalisée qu'au sein d'un contexte médical précis.
Selon certains critères (par exemple l'âge ou le sexe), un patient n'a pas la même probabilité qu'un autre d'être touché par une pathologie précise.
Le contexte du patient doit donc être précisé. 
Chaque contexte est représenté par un entier.

```
1 
```

### Base séquentielle

La base séquentielle est l'ensemble des données patients récoltées.
Elle comporte une suite de motifs de patients qui ont été enregistrés et regroupés par contexte.
Chaque contexte est représenté par un 'c'.
Le premier 'c' représente l'entrée dans le contexte 0, le second 'c' l'entrée dans le contexte 1, etc.
Chaque ligne représente le motif d'un patient au sein de son contexte.
Voici un exemple de base séquentielle :

```
'c'
1 2 3
4 5 6
1 4 2
'c'
1 4 2
7 8 
2
'c'
1 8
6
```

# Application

### Avertissement

La qualité de la prédiction dépend de la qualité de la base séquentielle.

Soit p1, le patient dont on veut déterminer l'hospitalisation future.

La prédiction se base sur les suites hospitalières des patients dont le motif comprend une correspondance partielle ou totale avec le motif de p1.
L'application ne détermine donc pas avec exactitude l'hospitalisation future de p1, mais analyse statistiquement les suites hospitalières des patients qui présentent une similitude avec lui.
Il est possible que p1 ne présente pas de suite hospitalière. Ce cas de figure n'est pas pris en compte dans notre application.

### Utilisation

Pour utiliser l'application, veuillez télécharger le code.

Pour compiler, veuillez utiliser la commande suivante :

```
make
```

Pour exécuter, veuillez utiliser la commande suivante :

```
./prediction contexte motif
```
