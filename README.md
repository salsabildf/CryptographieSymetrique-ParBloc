# Cryptographie symétrique

Ce projet à été réalisé en C, il consiste dans un premier temps en l'implémentation d'un générateur de type Geffe pour le chiffrement à flot, afin de pouvoir simuler une attaque par corrélation.
Dans un second temps, il consiste en l'implémentation d'un chiffrement par bloc de type de Feistel, sur 1 tour puis sur 12 pour permettre de procéder à une attaque sur 1 tour afin de la généraliser sur les tours complets.

## Pour commencer

Veuillez suivre les étapes qui suivent une par une, qui vous permettrons d'installer les utilitaires nécessaires au projet, de compilé le projet et de l'éxécuté correctement.

### Pré-requis

Pour le bon fonctionnement du programme il est impératifs de disposer des outils qui suivent :
- gcc 9.2.1 (ou version antérieur) 

```
sudo apt-get install gcc
```

- logiciel GNU Make 

```
sudo apt-get install build-essential
```


### Installation

* Pour installer le projet complet via git il faut entrer dans un terminal la commande git suivante :

```
git clone https://github.com/Zarakinoa/Cryptographie-Symetrique.git
``` 

>Si vous ne disposez pas de git, utiliser le lien qui suit => [Télécharger git](https://gist.github.com/derhuerst/1b15ff4652a867391f03)

* Si vous disposez de l'archive du projet, veuillez le décompréssé avec cette commande :

```
tar -cvf CryptographieSymetrique.tar CryptographieSymetrique/
```

## Démarrage

* Pour lancer le projet, vous devez vous rendre dans le dossier de celui-ci :

```
cd CryptographieSymetrique/
```
    
* Une fois à l'intérieur, éxecuter le logiciel GNU Make qui permettra de compiler la permiere partie (les arguments se font dans le Makefile et dans le main) :

```
make geffe
```
* et pour la deuxieme partie :
```
make feistel
```
* le spectre est generé dans le fichier "spectre.txt" :
```
make spectre
```


## Fabriqué avec

Pour le dévellopement de notre projet nous avons utiliser les ressources suivantes :

* [Visual Studio Code](https://code.visualstudio.com/) - Editeur de textes

## Auteurs

Voici la liste des personnes qui ont réalisés ce projet :

* **Salsabil Dafrane** _alias_ [@Salsa](https://github.com/salsabildf)
* **Théo Lefebvre** _alias_ [@Cobb](https://github.com/Zarakinoa)
* **Soufiane Chikar** _alias_ [@Ysejal](https://github.com/Ysejal)
