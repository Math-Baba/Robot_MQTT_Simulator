# Course de Robots - Système de Contrôle Temps Réel

## Description

**Course de Robots** est une application web complète permettant de simuler le contrôle et de monitoring de plusieurs robots en temps réel. Ce système utilise une architecture MQTT pour la communication avec les microcontrôleurs ESP32, associée à une interface web interactive basée sur Flask et WebSocket.

### Caractéristiques principales

**Contrôle multi-robots** - Gérez plusieurs robots simultanément depuis une seule interface  
**Temps réel** - Communication instantanée via WebSocket et MQTT  
**Monitoring en direct** - Suivez la vitesse et l'état de la batterie de chaque robot  
**Simulation ESP32** - Code Arduino compatible Wokwi pour la simulation ou déploiement réel  
**Interface responsive** - Interface web simple et intuitive  
**Architecture distribuée** - Séparation clean entre contrôleur et robots  

## Architecture

```
┌─────────────────────────────────────────────────────┐
│           Interface Web (HTML/JavaScript)            │
│          - Contrôle des robots                       │
│          - Affichage temps réel                      │
└──────────────────┬──────────────────────────────────┘
                   │ WebSocket
                   ▼
┌─────────────────────────────────────────────────────┐
│       Serveur Flask + Flask-SocketIO (Python)       │
│          - Réception des données temps réel         │
│          - Routage des commandes                    │
└──────────────────┬──────────────────────────────────┘
                   │ MQTT
                   ▼
┌─────────────────────────────────────────────────────┐
│        Broker MQTT (test.mosquitto.org)              │
└──────────────┬──────────────┬──────────────────────┘
               │              │
        ┌──────▼──┐      ┌────▼──────┐
        │ Robot 1 │      │ Robot 2   │
        │ (ESP32) │      │ (ESP32)   │
        └─────────┘      └───────────┘
```

## Technologies utilisées

### Backend
- **Python 3.8+**
- **Flask** - Framework web léger
- **Flask-SocketIO** - Communication WebSocket bidirectionnelle
- **Paho MQTT** - Client MQTT pour la communication avec les robots

### Frontend
- **HTML5**
- **JavaScript (ES6+)**
- **Socket.IO** - Librairie JavaScript pour WebSocket

### Robots
- **Arduino IDE / PlatformIO**
- **ESP32** - Microcontrôleur WiFi
- **PubSubClient** - Client MQTT Arduino

## Installation

### Prérequis

- **Python 3.8 ou supérieur** - Vérifier avec `python --version`
- **pip** - Gestionnaire de paquets Python
- **Git** (optionnel) - Pour cloner le projet

### Cloner ou télécharger le projet

```bash
# Avec Git
git clone https://github.com/Math-Baba/robot-mqtt-simulator.git
cd robot-mqtt-simulator
```

### 2️ Créer un environnement virtuel (recommandé)

```bash
python -m venv venv
venv\Scripts\activate
```

### 3️ Installer les dépendances Python

```bash
pip install -r requirements.txt
```

### Configuration des robots (Arduino/ESP32)

#### Pour la simulation Wokwi :

1. Accédez à [Wokwi.com](https://wokwi.com)
2. Créez un nouveau projet ESP32
3. Copiez le code depuis `eps32_simulation/sketch_jan6a/sketch_jan6a.ino`
4. Collez dans l'éditeur Wokwi
5. Changez `ROBOT_ID` si nécessaire (1, 2, etc.)

#### Pour le déploiement réel :

1. Téléchargez [Arduino IDE](https://www.arduino.cc/en/software) ou [PlatformIO](https://platformio.org/)
2. Installez le support ESP32
3. Ouvrez `sketch_jan6a.ino` ou `sketch_jan6c.ino`
4. Modifiez l'identifiant WiFi et le mot de passe
5. Modifiez `ROBOT_ID` pour identifier chaque robot
6. Téléversez sur l'ESP32

**Identification des robots :**
- Robot 1 : `ROBOT_ID = 1`
- Robot 2 : `ROBOT_ID = 2`
- Robot N : `ROBOT_ID = N`

## Guide d'utilisation

### Démarrage du serveur

```bash
# Assurez-vous que l'environnement virtuel est activé
# (Windows) venv\Scripts\activate
# (macOS/Linux) source venv/bin/activate

# Lancez l'application
python app.py
```

### Accès à l'interface web

1. Ouvrez votre navigateur
2. Accédez à `http://localhost:5000`

Vous verrez l'interface de contrôle avec les deux robots.

### Contrôler les robots

#### Via l'interface web

Pour chaque robot, trois boutons sont disponibles :

| Bouton | Action | Effet |
|--------|--------|-------|
| 🔼 **Accélérer** | `speed_up` | Augmente la vitesse de 5 km/h |
| 🔽 **Décélérer** | `speed_down` | Diminue la vitesse de 5 km/h |
| ⏹️ **Stop** | `stop` | Arrête le robot (vitesse = 0) |

#### Informations affichées

- **Vitesse** : Vitesse actuelle du robot en km/h
- **Batterie** : Niveau de la batterie en pourcentage

### Flux de communication

```
1. Utilisateur clique sur "Accélérer Robot 1"
   ↓
2. JavaScript envoie un événement WebSocket : 
   { robot: 1, command: "speed_up" }
   ↓
3. Flask reçoit et publie sur MQTT :
   Topic: cars/1/command
   Message: speed_up
   ↓
4. ESP32 Robot 1 reçoit la commande
   ↓
5. Augmente sa vitesse et envoie l'état :
   Topic: cars/1/status
   Message: speed: 15, battery: 95
   ↓
6. Flask reçoit via MQTT et le renvoie aux clients WebSocket
   ↓
7. Interface web met à jour l'affichage en temps réel
```

## Structure du projet

```
Course de robot/
├── app.py                          # Application Flask principale
├── requirements.txt                # Dépendances Python
├── README.md                      # Ce fichier
├── templates/
│   └── index.html                 # Interface web
└── eps32_simulation/
    ├── sketch_jan6a/
    │   └── sketch_jan6a.ino       # Code Arduino Robot 1
    └── sketch_jan6c/
        └── sketch_jan6c.ino       # Code Arduino Robot 2
```

## Configuration avancée

### Changer le broker MQTT

Si vous souhaitez utiliser votre propre broker MQTT au lieu de `test.mosquitto.org` :

**Dans `app.py` (ligne ~23) :**
```python
client.connect("votre-broker.com", 1883, 60)
```

**Dans les fichiers Arduino :**
```cpp
const char* mqtt_server = "votre-broker.com";
```

### Changer le port de l'application

**Dans `app.py` (dernière ligne) :**
```python
socketio.run(app, host="0.0.0.0", port=8080)  # Port 8080 au lieu de 5000
```

### Ajouter plus de robots

1. Créez un nouveau sketch Arduino avec `ROBOT_ID` différent
2. Téléversez sur un autre ESP32
3. Ajoutez une nouvelle section HTML dans `index.html` :

```html
<div id="robot-3">
    <h3>Robot 3</h3>
    <p>Vitesse : <span id="robot-3-speed">--</span></p>
    <p>Batterie : <span id="robot-3-battery">--</span></p>
    <button onclick="sendCommand(3, 'speed_up')">Accélérer</button>
    <button onclick="sendCommand(3, 'speed_down')">Décélérer</button>
    <button onclick="sendCommand(3, 'stop')">Stop</button>
</div>
```
