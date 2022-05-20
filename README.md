Student - Adrien Paysant - Forked from flassabe

# Sockets

Ce TD a pour but de vous familiariser progressivement à la programmation d'applications reposant sur des communications réseau basées sur des sockets.

L'implémentation sera faite dans un premier temps en C, puis en C++ dans les derniers exercices.

Ce sujet évoluera au fil du temps pour couvrir les bases des sockets, puis le multiplexage, et finalement l'usage de Qt.

Les instructions et la base de code éventuelle seront partagées via des dépôts git. Si vous n'êtes pas à l'aise avec cet outil, vous pouvez vous référer au cours de GL41 disponible [ici](https://www.youtube.com/watch?v=jJBODtE_Rts&list=PLNgzB9uJ0Ss58ZdoPk1vueOMYYdfBkKoS)

Les vidéos du cours sur les sockets publiées sur Moodle sont également disponibles sur [Youtube](https://www.youtube.com/watch?v=4goPrs4NtOk&list=PLNgzB9uJ0Ss4nsIBJV8j8XJwYzwV43nk2)

## Paramétrisation et sécurisation d'un client TCP

Dans cet exercice, vous vous baserez sur l'exemple de client TCP vu en cours pour le rendre paramétrable :

- l'option `h` permet de définir l'hôte auquel se connectera le client.
- l'option `p` définira le port d'écoute du serveur auquel le client va se connecter.

Vous vérifierez les retours des fonctions utilisées dans le programme pour permettre de quitter proprement en cas de problème (serveur inexistant, etc.)

## Serveur UDP avec trace des réceptions

Dans cet exercice, vous vous baserez sur le serveur UDP du cours, qu'il s'agira dans un premier temps de rendre plus robuste (vérification des valeurs de retour des fonctions), avant de permettre de récupérer l'adresse et le port source des envois reçus. (il faut passer des paramètres non `NULL` au deux derniers paramètres de recvfrom)

## Serveur TCP full duplex

On considérera toujours un serveur TCP à un seul client. Celui ci devra ajouter un texte à celui envoyé par son client et lui répondra par ce texte complété. Là encore, il est nécessaire de mémoriser l'adresse et le port du client connecté.

## Chat avec un serveur TCP multiplexé

Cet exercice reposera sur le multiplexage des communications pour permettre le support de plusieurs clients en même temps. L'application cliente fonctionne en deux étapes : tout d'abord le programme demande à l'utilisateur quel sera son pseudo, et se met en attente de l'entrée de l'utilisateur. Une fois le pseudo saisi, le client se connecte au serveur et boucle sur la saisie utilisateur, qui sera concaténée au pseudo de l'utilisateur afin d'être envoyée au serveur. À la réception d'un message, le client l'affiche sur la sortie standard.

Le serveur fonctionne en se mettant en attente de connexions. À chaque client connecté, le serveur ajoute ce dernier dans sa liste de clients. À la réception d'un message, le serveur le retransmet à tous les clients sauf celui qui était à l'origine du message. Le serveur affiche également tous les messages reçus.

Vous multiplexerez les accès avec la fonction `select`.
