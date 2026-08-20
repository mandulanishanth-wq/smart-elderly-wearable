#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <MAX30105.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <MPU6050.h>
#include <TinyGPS++.h>

#define M_PI 3.14159265358979323846264338327950288

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

WebServer server(80);

Adafruit_SH1106G display(128,64,&Wire,-1);

MAX30105 particleSensor;
MPU6050 mpu;

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

const double fence[10][2] = {
{17.529188,78.361845},
{17.529840,78.361919},
{17.529934,78.362197},
{17.530624,78.362507},
{17.530832,78.363043},
{17.530999,78.363451},
{17.530924,78.363976},
{17.529248,78.363288},
{17.529101,78.362858},
{17.529040,78.362489}
};

int fenceSize = 10;
int targetStatus = 0;

double latitude = 0;
double longitude = 0;
int sat;

unsigned long lastBeatTime = 0;
int bpm = 0;
long prevIR = 0;

#define PEAK_THRESHOLD 300
#define IR_FINGER_MIN 8000

int bpmBuffer[5];
int bpmIndex = 0;
bool bufferFilled = false;

float spo2 = 0;
float redAC = 0, irAC = 0;
float redDC = 0, irDC = 0;

bool fallLatched = false;

#define FALL_THRESHOLD 2

String webpage = R"====(
<!DOCTYPE html>
<html>
<head>
<title>Patient Health Monitor</title>
<style>
body{
font-family:Arial;
background:#0f172a;
color:white;
text-align:center;
}
.card{
background:#1e293b;
margin:15px;
padding:20px;
border-radius:10px;
font-size:22px;
}
</style>
<script>
setInterval(function(){
fetch("/data")
.then(response=>response.json())
.then(data=>{
document.getElementById("bpm").innerHTML=data.bpm
document.getElementById("spo2").innerHTML=data.spo2
document.getElementById("fall").innerHTML=data.fall
document.getElementById("geo").innerHTML=data.geo
document.getElementById("lat").innerHTML=data.lat
document.getElementById("lon").innerHTML=data.lon
})
},1000)
</script>
</head>
<body>
<h1>Health Monitoring System</h1>
<div class="card">BPM : <span id="bpm">0</span></div>
<div class="card">SpO2 : <span id="spo2">0</span></div>
<div class="card">Fall : <span id="fall">NORMAL</span></div>
<div class="card">Geofence : <span id="geo">INSIDE</span></div>
<div class="card">Latitude : <span id="lat">0</span></div>
<div class="card">Longitude : <span id="lon">0</span></div>
</body>
</html>
)====";

void pip()
{
double vectors[fenceSize][2];
for(int i=0;i<fenceSize;i++)
{
vectors[i][0] = fence[i][0] - latitude;
vectors[i][1] = fence[i][1] - longitude;
}
double angle = 0;
for(int i=0;i<fenceSize;i++)
{
double num =
(vectors[i%fenceSize][0])*(vectors[(i+1)%fenceSize][0]) +
(vectors[i%fenceSize][1])*(vectors[(i+1)%fenceSize][1]);

double den =
sqrt(pow(vectors[i%fenceSize][0],2)+pow(vectors[i%fenceSize][1],2)) *
sqrt(pow(vectors[(i+1)%fenceSize][0],2)+pow(vectors[(i+1)%fenceSize][1],2));

angle += 180*acos(num/den)/M_PI;
}
if(angle > 355 && angle < 365)
targetStatus = 1;
else
targetStatus = 0;
}

void handleRoot()
{
server.send(200,"text/html",webpage);
}

void handleData()
{
String json="{";
json += "\"bpm\":" + String(bpm) + ",";
json += "\"spo2\":" + String(spo2,1) + ",";
json += "\"fall\":\"" + String(fallLatched?"FALL DETECTED":"NORMAL") + "\",";
json += "\"geo\":\"" + String(targetStatus?"INSIDE":"OUTSIDE") + "\",";
json += "\"lat\":" + String(latitude,6) + ",";
json += "\"lon\":" + String(longitude,6);
json+="}";
server.send(200,"application/json",json);
}

void setup()
{
Serial.begin(115200);
Wire.begin(21,22);
display.begin(0x3C,true);
display.setTextColor(SH110X_WHITE);
particleSensor.begin(Wire);
particleSensor.setup();
mpu.initialize();
gpsSerial.begin(9600,SERIAL_8N1,16,17);

WiFi.begin(ssid,password);
while(WiFi.status()!=WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println(WiFi.localIP());

server.on("/",handleRoot);
server.on("/data",handleData);
server.begin();
}

void loop()
{
server.handleClient();

while(gpsSerial.available())
gps.encode(gpsSerial.read());

if(gps.location.isValid())
{
latitude = gps.location.lat();
longitude = gps.location.lng();
pip();
}

long irValue = particleSensor.getIR();
long redValue = particleSensor.getRed();

long diff = irValue-prevIR;
prevIR=irValue;

if(irValue < IR_FINGER_MIN)
{
bpm=0;
spo2=0;
}
else
{
if(diff > PEAK_THRESHOLD)
{
unsigned long now = millis();
if(lastBeatTime>0)
{
unsigned long delta = now-lastBeatTime;
if(delta>330 && delta<1500)
{
int instantBPM = 60000/delta;
bpmBuffer[bpmIndex++] = instantBPM;
if(bpmIndex>=5)
{
bpmIndex=0;
bufferFilled=true;
}
int sum=0;
int count= bufferFilled?5:bpmIndex;
for(int i=0;i<count;i++)
sum+=bpmBuffer[i];
bpm=sum/count;
}
}
lastBeatTime=now;
}

redDC = 0.95*redDC + 0.05*redValue;
irDC  = 0.95*irDC  + 0.05*irValue;
redAC = redValue-redDC;
irAC  = irValue-irDC;

if(irAC>0 && redAC>0)
{
float R = (redAC/redDC)/(irAC/irDC);
spo2 = 110-25*R;
if(spo2>100) spo2=100;
if(spo2<90) spo2=90;
}
}

int16_t ax,ay,az;
mpu.getAcceleration(&ax,&ay,&az);

float Ax=ax/16384.0;
float Ay=ay/16384.0;
float Az=az/16384.0;
float accelMag = sqrt(Ax*Ax + Ay*Ay + Az*Az);

if(accelMag > FALL_THRESHOLD)
fallLatched=true;

display.clearDisplay();
display.setCursor(0,0);
display.println("Elderly Monitor");
display.setCursor(0,15);
display.print("BPM:");
display.println(bpm);
display.setCursor(0,30);
display.print("SpO2:");
display.println(spo2);
display.setCursor(0,45);
display.println(targetStatus?"INSIDE ZONE":"OUTSIDE ZONE");
display.display();

delay(10);
}
