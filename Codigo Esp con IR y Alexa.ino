#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProTV.h>
#include <SinricProSpeaker.h>
#include <SinricProLight.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// ==========================================
// 1. CREDENCIALES Y DISPOSITIVOS
// ==========================================
#define WIFI_SSID         "iPhone de Giovanny"
#define WIFI_PASS         "hola1234"
#define APP_KEY           "c0cb06c5-2484-4381-936f-a093fd04ebcc"
#define APP_SECRET        "0e38952e-b548-443a-b130-b3a503408dbd-d7d010c2-6867-4148-afc3-0fa186cf814d"

#define TV_ID             "6a014a18c93c9fd0e0f02359" // Tu TV LG
#define BARRA_ID          "6a0153fdc93c9fd0e0f028b7" // Tu Soundbar
#define LUCES_ID          "6a015438f9b5f15fa7d22c55" // Tus Luces LED

// ==========================================
// 2. HARDWARE IR
// ==========================================
const uint16_t kIrLed = 5; 
IRsend irsend(kIrLed);

// ==========================================
// 📺 CALLBACKS DE LA TV (Protocolo NEC - 32 bits)
// ==========================================
bool onPowerStateTV(const String &deviceId, bool &state) {
  Serial.printf("TV: %s\n", state ? "ON" : "OFF");
  irsend.sendNEC(0x20DF10EF, 32); 
  return true;
}
bool onAdjustVolumeTV(const String &deviceId, int &volumeDelta, bool volumeDefault) {
  if (volumeDelta > 0) {
    Serial.println("TV: Subir Volumen");
    irsend.sendNEC(0x20DF40BF, 32);
  } else {
    Serial.println("TV: Bajar Volumen");
    irsend.sendNEC(0x20DFC03F, 32);
  }
  return true;
}
bool onSkipChannelsTV(const String &deviceId, int channelDelta, String &channelName) {
  if (channelDelta > 0) {
    Serial.println("TV: Subir Canal");
    irsend.sendNEC(0x20DF00FF, 32);
  } else {
    Serial.println("TV: Bajar Canal");
    irsend.sendNEC(0x20DF807F, 32);
  }
  channelName = "Canal cambiado"; 
  return true;
}

// ==========================================
// 🔊 CALLBACKS DE LA BARRA (Protocolo SAMSUNG36 - 36 bits)
// ==========================================
bool onPowerStateBarra(const String &deviceId, bool &state) {
  Serial.printf("Barra: %s\n", state ? "ON" : "OFF");
  irsend.sendSamsung36(0xCF000EF1, 36); 
  return true;
}
bool onAdjustVolumeBarra(const String &deviceId, int &volumeDelta, bool volumeDefault) {
  if (volumeDelta > 0) {
    Serial.println("Barra: Subir Volumen");
    irsend.sendSamsung36(0xCF00EE11, 36);
  } else {
    Serial.println("Barra: Bajar Volumen");
    irsend.sendSamsung36(0xCF001EE1, 36);
  }
  return true;
}
bool onMuteBarra(const String &deviceId, bool &mute) {
  Serial.printf("Barra Mute: %s\n", mute ? "ON" : "OFF");
  irsend.sendSamsung36(0xCF008E71, 36); 
  return true;
}

// TRAMPA: CONTROLES DE REPRODUCCIÓN MULTIPLEXADOS A BAJOS
bool onMediaControlBarra(const String &deviceId, String &control) {
  if (control == "Next" || control == "FastForward") { 
    Serial.println("Barra: Subir Bajos (Comando: Siguiente)");
    irsend.sendSamsung36(0xCF0041BE, 36); 
  } 
  else if (control == "Previous" || control == "Rewind") { 
    Serial.println("Barra: Bajar Bajos (Comando: Anterior)");
    irsend.sendSamsung36(0xCF00C13E, 36); 
  }
  else if (control == "Stop") { 
    Serial.println("Barra: Bajos en Cero (Comando: Detener)");
    irsend.sendSamsung36(0xCF005EA1, 36); 
  }
  return true;
}

// ==========================================
// 💡 CALLBACKS DE LAS LUCES LED (Protocolo NEC - 32 bits)
// ==========================================
bool onPowerStateLuces(const String &deviceId, bool &state) {
  Serial.printf("Luces: %s\n", state ? "ON" : "OFF");
  if (state) irsend.sendNEC(0xF7C03F, 32); 
  else irsend.sendNEC(0xF740BF, 32);       
  return true;
}
bool onColorLuces(const String &deviceId, byte &r, byte &g, byte &b) {
  // Colores Básicos
  if (r > 200 && g < 100 && b < 100) {
     Serial.println("Luces: Rojo");
     irsend.sendNEC(0xF720DF, 32);
  } 
  else if (g > 200 && r < 100 && b < 100) {
     Serial.println("Luces: Verde");
     irsend.sendNEC(0xF7A05F, 32);
  } 
  else if (b > 200 && r < 100 && g < 100) {
     Serial.println("Luces: Azul");
     irsend.sendNEC(0xF7609F, 32);
  }
  // TRAMPA: COLORES MULTIPLEXADOS A PATRONES
  else if (r > 200 && g > 200 && b < 100) {
     Serial.println("Activando Patrón 1 (Comando: Amarillo)");
     irsend.sendNEC(0xF7D02F, 32);
  }
  else if (r > 200 && g < 100 && b > 200) {
     Serial.println("Activando Patrón 2 (Comando: Morado/Magenta)");
     irsend.sendNEC(0xF7F00F, 32);
  }
  else if (r < 100 && g > 200 && b > 200) {
     Serial.println("Activando Patrón 3 (Comando: Cian/Celeste)");
     irsend.sendNEC(0xF7C837, 32);
  }
  else if (r > 200 && g > 200 && b > 200) {
     Serial.println("Activando Patrón 4 (Comando: Blanco)");
     irsend.sendNEC(0xF7E817, 32);
  }
  return true;
}

// ==========================================
// CONFIGURACIÓN PRINCIPAL
// ==========================================
void setup() {
  Serial.begin(115200);
  irsend.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando al Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\n¡Conectado al Wi-Fi!");

  // 1. Inicializar TV
  SinricProTV &myTV = SinricPro[TV_ID];
  myTV.onPowerState(onPowerStateTV);
  myTV.onAdjustVolume(onAdjustVolumeTV);
  myTV.onSkipChannels(onSkipChannelsTV);

  // 2. Inicializar Barra de Sonido
  SinricProSpeaker &myBarra = SinricPro[BARRA_ID];
  myBarra.onPowerState(onPowerStateBarra);
  myBarra.onAdjustVolume(onAdjustVolumeBarra);
  myBarra.onMute(onMuteBarra);
  myBarra.onMediaControl(onMediaControlBarra); 

  // 3. Inicializar Luces LED
  SinricProLight &myLuces = SinricPro[LUCES_ID];
  myLuces.onPowerState(onPowerStateLuces);
  myLuces.onColor(onColorLuces);

  SinricPro.onConnected([](){ Serial.println("¡Conectado a Sinric Pro! 3 Dispositivos Activos."); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();
}