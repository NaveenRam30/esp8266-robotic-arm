#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

const char* ssid = "RobotArm";
const char* password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Servo claw;
Servo topArm;
Servo bottomArm;
Servo swing;

int clawPos = 10;
int topPos = 100;
int bottomPos = 180;
int swingPos = 180;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slider { width: 100%; }
</style>
</head>
<body>
<h2>Robot Arm Control</h2>

<p>Claw</p>
<input type="range" min="0" max="45" value="0" class="slider"
oninput="send('claw', this.value)">

<p>Top Arm</p>
<input type="range" min="0" max="180" value="0" class="slider"
oninput="send('top', this.value)">

<p>Bottom Arm</p>
<input type="range" min="0" max="180" value="0" class="slider"
oninput="send('bottom', this.value)">

<p>Swing</p>
<input type="range" min="0" max="180" value="90" class="slider"
oninput="send('swing', this.value)">

<script>
var ws = new WebSocket("ws://" + location.hostname + "/ws");
function send(name, value){
  ws.send(name + "," + value);
}
</script>
</body>
</html>
)rawliteral";


// ================== SERVO LOGIC ==================

void handleServo(String name, int value)
{
  if (name == "claw")
  {
    if (value > clawPos && clawPos < 45) clawPos++;
    else if (value < clawPos && clawPos > 0) clawPos--;
    claw.write(clawPos);
  }

  else if (name == "top")
  {
    if (value > topPos && topPos < 180) topPos += 3;
    else if (value < topPos && topPos > 0) topPos -= 3;
    topArm.write(topPos);
  }

  else if (name == "bottom")
  {
    if (value > bottomPos && bottomPos < 180) bottomPos += 3;
    else if (value < bottomPos && bottomPos > 0) bottomPos -= 3;
    bottomArm.write(bottomPos);
  }

  else if (name == "swing")
  {
    if (value > swingPos && swingPos < 180) swingPos += 3;
    else if (value < swingPos && swingPos > 0) swingPos -= 3;
    swing.write(swingPos);
  }
}


// ================== WEBSOCKET ==================

void onWsEvent(AsyncWebSocket *server,
               AsyncWebSocketClient *client,
               AwsEventType type,
               void *arg,
               uint8_t *data,
               size_t len)
{
  if(type == WS_EVT_DATA)
  {
    String msg = "";
    for(size_t i=0;i<len;i++)
      msg += (char)data[i];

    int comma = msg.indexOf(',');
    String name = msg.substring(0, comma);
    int value = msg.substring(comma+1).toInt();

    handleServo(name, value);
  }
}


// ================== SETUP ==================

void setup()
{
  Serial.begin(115200);

  claw.attach(D4);
  topArm.attach(D3,500,2400);
  bottomArm.attach(D7,500,2400);
  swing.attach(D8,500,2400);

  claw.write(clawPos);
  topArm.write(topPos);
  bottomArm.write(bottomPos);
  swing.write(swingPos);

  WiFi.softAP(ssid, password);

  Serial.println(WiFi.softAPIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}


// ================== LOOP ==================

void loop()
{
  ws.cleanupClients();
}
