/*  Dendrometro de precisão para mensurar tamanho inicial e 
 *   variações de crescimento do diâmetro das árvores 
 *   (DAP - diametro a altura do peito com casca). 
 *  
 *  Code by 3ecologias.net <root@3ecologias.net>
 *  
 *  notes:
 *  1 lap lapV =~ 0.5 volts (v) vcc 5v
 *  1 lap lapC =~ 7.9 cm
 *  perimetro extra cE =~ 4 cm
 *  perimetro total cT =~ 64 cm
 *  diametro inicial dI =~ 1.2732 cm
 *  diametro final dT =~ 20.3718 cm
 *  led vermelho: pre-calibragem 
 *  led verde: modo de cálculo do diâmetro (DAP) e do crescimento (ICAd) em cm.
 *  
 *  steps:
 *  1. ajuste o carretel do sensor e acople o dispositivo na árvore com o dispositivo desligado;
 *  2. ligue o dispositivo para habilitar o modo de calibragem, medição inicial do diâmetro, crescimento e catalogo de dados;
 *  3. o modo de calibragem do sensor verifica se o carretel realizou no mímino uma volta completa;
 *  4. o modo de medição do DAP e ICAd captura por 20 segundos o valor inicial do diâmetro;
 *  5. o modo de crescimento coleta, a cada minuto, o valor do diâmetro (DAP) e compara com o último valor do DAP para registrar o incremento do diâmetro (ICAd);
 *  6. o modo de catalogo armazena, a cada minuto, um log em CSV no SDCard com os valores de hora, data e o DAP: inicial, final e o ICAd. 
 *  7. Para salvar o log, basta voltar o carretel para a posição inicial e aguardar o led vermelho acender. Depois disso, desligue o dispositivo e remova o SDCard.
*/

// =*= RTC CLOCK DS1307 AND SDCARD CONFIG =*=

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#include <SdFat.h>

// SPI pinout
#define PIN_SPI_CLK 13
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SD_CS 4

#define PIN_OTHER_DEVICE_CS -1

#define SD_CARD_SPEED SPI_FULL_SPEED 

#define FILENAME "datalogger.csv"

SdFat sd;
SdFile csv;

// =*= END SDCARD CONFIG =*=

// scope of constant
const int sensorPin = A0;    // pin that the dendro string pot sensor is attached to
const int ledPre = 6;
const int ledCap = 5;
const float Pi = 3.14159265359;

// scope of variables

// modules 
bool record = true;
bool calibrate = true;
bool measurement = false;
bool growth = false;
bool catalog = false;
bool calibrateState = true;
bool meansurementState = false;

// global variables
float sensorValue = 0;
float lapC = 780; // perimetro (cm) x 100
float lapV = 49; // volts (V) x 100
float cE = 200; // perimetro (cm) x 100
float baseline = 49; // perimetro externo inicial
int laps = 0;

float cI = 0; // perimetro inicial (cm)
float cL = 0; // perimetro final (cm)
float dI = 0; // diametro inicial (cm)
float dL = 0; // diametro final (cm)
float dapI = 0; // diametro inicial a altura do peito (cm)
float dapL = 0; // diametro final a altura do peito (cm)
float ICAd = 0.0000; // incremento de diametro (cm)
float voltage = 0;
String dataLogger ="";
tmElements_t tm;

void setup() {
  // Setup SdCard Pins
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_CLK, OUTPUT);
  //Disable SPI devices
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);
  
  #if PIN_OTHER_DEVICE_CS > 0
    pinMode(PIN_OTHER_DEVICE_CS, OUTPUT);
    digitalWrite(PIN_OTHER_DEVICE_CS, HIGH);
  #endif //PIN_OTHER_DEVICE_CS > 0
 
 Serial.begin(9600);
 // turn on LED to signal the start of the calibration and capture period:
 pinMode(ledPre, OUTPUT);
 pinMode(ledCap, OUTPUT);
 

 // Setup SD card boot
  if (!sd.begin(PIN_SD_CS, SD_CARD_SPEED)) {
    Serial.println("SD card begin error");
    return;
  }
}
// sdcard create csv file
void initSdFile()
{
  if (sd.exists(FILENAME) && !sd.remove(FILENAME))
  {
    Serial.println("Failed init remove file");
    return;
  }
  if (!csv.open(FILENAME, O_RDWR | O_CREAT)) {
    Serial.println("Failed open file");
  } else {
    Serial.println("Sdcard saving!");
  }
}

void loop() {

  Serial.println("===== IOTr3es =====");
  Serial.println("===== BUILDING MODULES =====");
  sensorValue = analogRead(sensorPin);
  voltage = (sensorValue * (5.0 / 1023.0)) * 100;
  
  // calibration mode
 while (calibrate == true) {
  Serial.println("===== CALIBRATE MODE =====");
  sensorValue = analogRead(sensorPin);
  voltage = (sensorValue * (5.0 / 1023.0)) * 100;
  digitalWrite(ledPre, HIGH); 
  // check pot minimum sensor value
   if (voltage >= baseline) {
    for (int i = 0; i < 10; i++) {
     voltage = (sensorValue * (5.0 / 1023.0)) * 100;
     cI = getcI(lapC, lapV, voltage);
     cI = cI + cE;
     dI = cI / Pi;
     Serial.println("===== CALIBRATE SENSOR =====");
     delay(100);
    }
    digitalWrite(ledPre, LOW);
    calibrate = false;
    measurement = true;
   }
   Serial.println("===== CALIBRATE WAITING =====");
   Serial.println("voltage" );
   Serial.println(rawData(voltage));
   delay(1000);
 }
 
  // meansurement mode
 while (measurement == true) {
  Serial.println("===== MEANSUREMENT MODE =====");
  digitalWrite(ledCap, HIGH);
  sensorValue = analogRead(sensorPin);
  voltage = (sensorValue * (5.0 / 1023.0)) * 100;
  // check pot minimum sensor value
   if (voltage >= baseline) {
    for (int i = 0; i < 20; i++) {
     sensorValue = analogRead(sensorPin);
     voltage = (sensorValue * (5.0 / 1023.0)) * 100;
     cI = getcI(lapC, lapV, voltage);
     cI = cI + cE;
     dapI = cI / Pi;
     delay(100);
    }
    measurement = false;
    growth = true;
   } else {
    measurement = false;
    calibrate = true;
   }
 }

// growth mode
 while (growth == true) {
  digitalWrite(ledCap, HIGH);
  for (int i = 0; i >= 60; i++) {
     digitalWrite(ledCap, LOW);
     delay(100);
     sensorValue = analogRead(sensorPin);
     voltage = (sensorValue * (5.0 / 1023.0)) * 100;
     digitalWrite(ledCap, HIGH);
     Serial.println("===== GROWTH TIME =====");
  }
  if (record == true) {
    Serial.println("===== INIT SDCARD MODULE =====");
    initSdFile();
    csv.println("HOUR, MINUTE, SECOND, DAY, MONTH, YEAR, DAP-I, DAP-L, ICAD");
    record = false;
  }
  // get dap last
  sensorValue = analogRead(sensorPin);
  voltage = (sensorValue * (5.0 / 1023.0)) * 100;
  // get last DAP value in cm
  cI = getcI(lapC, lapV, voltage);
  cL = cI + cE;
  dapL = cL / Pi;
  if (dapL > dapI) {
    ICAd = dapL - dapI;
     if (RTC.read(tm)) {    
        dataLogger = String(tm.Hour) + "," + String(tm.Minute) + "," + String(tm.Second) + "," + String(tm.Day) + "," + String(tm.Month) + "," + String(tmYearToCalendar(tm.Year)) + "," + String(rawData(dapI)) + "," + String(rawData(dapL)) + "," + String(rawData(ICAd));
        csv.println(dataLogger);
        Serial.println("Datalogger ok");
     }
  } else {
    ICAd = dapI - dapL;   
     if (RTC.read(tm)) {    
        dataLogger = String(tm.Hour) + "," + String(tm.Minute) + "," + String(tm.Second) + "," + String(tm.Day) + "," + String(tm.Month) + "," + String(tmYearToCalendar(tm.Year)) + "," + String(rawData(dapI)) + "," + String(rawData(dapL)) + "," + String(rawData(ICAd));
        csv.println(dataLogger);
        Serial.println("Datalogger ok");
     }
  }
  delay(10000);
  if (voltage <= baseline) {
     digitalWrite(ledCap, LOW);
     csv.close();
     Serial.println("SDCard saved!" );
     record = true;
     growth = false;
     calibrate = true;
  }
 }
}

// Function to get length
float getcI(float lapC, float lapV, float voltage){
  float _cIraw;
  float cIraw;
  _cIraw = lapC * voltage;
  cIraw = _cIraw / lapV;
  return cIraw;
}

// Function to convert data to cm
float rawData(float value){
  float rawD;
  rawD = value / 100;
  return rawD;
}

