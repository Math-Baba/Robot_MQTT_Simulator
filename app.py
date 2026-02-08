from flask import Flask, render_template
from flask_socketio import SocketIO
import paho.mqtt.client as mqtt

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode="threading")

def on_connect(client, userdata, flags, reason_code, properties):
    print("MQTT connecté")
    client.subscribe("cars/+/status")

def on_message(client, userdata, msg):
    data = msg.payload.decode()
    print("MQTT:", msg.topic, data)
    socketio.emit("robotData", {
        "topic": msg.topic,
        "data": data
    })

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect("test.mosquitto.org", 1883, 60)
client.loop_start()

@app.route("/")
def index():
    return render_template("index.html")

@socketio.on("sendCommand")
def handle_command(data):
    robot_id = data["robot"]
    command = data["command"]

    topic = f"cars/{robot_id}/command"
    client.publish(topic, command)

    print(f"Commande envoyée → {topic} : {command}")

if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=5000)
