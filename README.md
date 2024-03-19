# ESE_BMMPE_chat_souris

Ce project a pour but de développer un robot qui joue à chat avec d'autres robots, il embarque un PCB custom avec un STM32G070. Le projet est développé avec freeRTOS.

Le pcb permet la connection de :
- 2 moteurs DC
- 2 encodeurs
- 1 module Bluetooth
- 1 lidar ydlidar x4
- 4 capteurs de touche
- 2 capteurs de vide
- 1 centrale inertielle

Membres :

- M PICHERY
- B RODIER
- M VAN DE MOOSDJIK
- P.-E. RETAUX


## Organisation du code

Le code fonctionne en modules contenus dans le dossier `drivers`.
Les dossiers sont organisés de la manière suivante :

- `CONTROL` :
Gère la logique de contrôle du robot. Il génère les signaux de commande pour les moteurs et les encodeurs.
- `HC-05` :
Api de communication avec le module Bluetooth HC-05.
- `IT` :
Gère les interruptions du microcontrôleur.
- `LIDAR` : 
Routine d'acquisition des données du lidar.
- `MOTOR` :
Gère les moteurs et les encodeurs.
- `MPU6050` :
Api de communication avec la centrale inertielle MPU6050.
- `PARAMS` :
Gère les paramètres du robot.
- `SYS_HELPER` :
Fonctions utilitaires pour le système.
- `UART` :
Fonctions de communication UART.


