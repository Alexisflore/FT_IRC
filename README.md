# FT_IRC

## Description

FT_IRC est une implémentation personnalisée d'un serveur IRC (Internet Relay Chat) développé en C++. Ce projet recrée les fonctionnalités essentielles d'un serveur IRC, permettant la communication en temps réel via des canaux (channels) et des messages privés.

## Table des Matières

1. [Fonctionnalités](#fonctionnalités)
2. [Installation](#installation)
3. [Utilisation](#utilisation)
4. [Modes et Commandes](#modes-et-commandes)
5. [Structure du Projet](#structure-du-projet)
6. [Contributeurs](#contributeurs)
7. [Licence](#licence)
8. [Ressources](#ressources)

## Fonctionnalités

FT_IRC inclut les fonctionnalités suivantes :

- **Canaux**: Créez, rejoignez et gérez des canaux de discussion.
- **Messages Privés**: Envoyez des messages directs à d'autres utilisateurs.
- **Modes Utilisateur et Canal**: Appliquez des modes spécifiques aux utilisateurs et aux canaux (ex : opérateur, invité uniquement, etc.).
- **Gestion des Utilisateurs**: Enregistrez et gérez les utilisateurs connectés.
- **Protocoles de Sécurité**: Implémentation de mécanismes de sécurité de base.
- **Extensibilité**: Conçu pour être facilement extensible avec des fonctionnalités supplémentaires.

## Installation

Pour installer FT_IRC sur votre machine locale, suivez les étapes ci-dessous.

### Prérequis

- **C++98** ou version antérieur.
- **CMake** 3.10 ou version ultérieure.
- **Make** (ou un autre build system compatible).
- **Git** pour cloner le dépôt (optionnel mais recommandé).

### Instructions

1. **Cloner le Dépôt** :
   ```bash
   git clone https://github.com/votreutilisateur/FT_IRC.git
   ./ircserv

Compiler le Projet :

```bash
mkdir build
cd build
cmake ..
make
```
Exécuter le Serveur :

```bash
./ft_irc <port> <password>
```
Remplacez <port> par le numéro de port sur lequel le serveur doit écouter et <password> par le mot de passe nécessaire pour se connecter au serveur.

Utilisation
Connexion
Pour vous connecter au serveur IRC :

1.	Utilisez un client IRC (par exemple, HexChat, WeeChat, ou irssi).
2.	Configurez une nouvelle connexion avec l'adresse IP ou le nom de domaine de votre serveur, et le port spécifié lors du lancement du serveur.
3.	Entrez le mot de passe que vous avez spécifié.

Commandes IRC de Base
Une fois connecté, vous pouvez utiliser des commandes IRC standards :

Rejoindre un canal : `/join #canal`

Envoyer un message privé : `/msg <utilisateur> <message>`

Quitter un canal : /part `#canal`

Définir un sujet de canal : `/topic #canal <sujet>`

Changer de pseudonyme : `/nick <nouveau_nick>`

## Mode et commandes
Modes de Canal

`+t`: Seuls les opérateurs peuvent changer le sujet du canal.

`+i`: Le canal est en mode invitation seulement.

`+k <password>`: Nécessite un mot de passe pour rejoindre.

`+m`: Le canal est modéré (seuls les utilisateurs avec des privilèges peuvent parler).

Modes Utilisateur

`+i`: Invisible (l'utilisateur n'apparaît pas dans les listes de canaux pour les non-membres).

`+o`: Opérateur de canal.

`+v`: Voix (autorise à parler dans un canal modéré).

Commandes Admin
Kick : Éjecter un utilisateur d'un canal.
```bash
/kick <canal> <utilisateur> [raison]
```

Ban : Bannir un utilisateur d'un canal.
```bash
/mode <canal> +b <utilisateur>
```

Unban : Débannir un utilisateur d'un canal.
```bash
/mode <canal> -b <utilisateur>
```

## Structure du Projet
```bash
FT_IRC/
│
├── src/                    # Code source principal
│   ├── main.cpp            # Point d'entrée du programme
│   ├── server/             # Implémentation du serveur
│   ├── client/             # Gestion des clients
│   └── utils/              # Utilitaires divers
│
├── include/                # Fichiers d'en-tête
│   ├── server/             # En-têtes pour le serveur
│   ├── client/             # En-têtes pour la gestion des clients
│   └── utils/              # En-têtes pour les utilitaires
│
├── tests/                  # Tests unitaires
│
├── CMakeLists.txt          # Fichier de configuration de CMake
├── README.md               # Documentation du projet
└── LICENSE                 # Fichier de licence
```

## Contributeurs
Nom Prénom: Alexiflore

Nom Prénom: Petitco3ur

Les contributions sont les bienvenues ! Veuillez consulter notre guide de contribution pour plus de détails.

## Licence
Ce projet est sous licence MIT. Consultez le fichier LICENSE pour plus de détails.

## Ressources
Documentation IRC - RFC 1459 pour le protocole IRC.
C++ Reference - Documentation C++ standard.
Boost Libraries - Bibliothèques C++ complémentaires.
Merci d'utiliser FT_IRC ! Nous sommes impatients de recevoir vos retours et contributions.