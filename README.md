# SmartHome IR Gateway - ESP32 🏠📡

Nodo domótico centralizado basado en el microcontrolador ESP32 para la integración de electrodomésticos convencionales (Infrarrojos) con el ecosistema de **Amazon Alexa**.

## 📌 Descripción de la Arquitectura
Este proyecto actúa como un "Gateway" o puente de telecomunicaciones (Edge Node). Convierte directivas de voz de procesamiento de lenguaje natural (NLP) en señales de control infrarrojo estandarizadas, operando a través de una red Wi-Fi y el protocolo MQTT mediante la plataforma middleware **Sinric Pro**.

### ✨ Características Principales
* **Protocolo NEC (32-bit):** Integración completa para el control de un Televisor LG y un sistema de iluminación LED RGB.
* **Protocolo SAMSUNG36 (36-bit):** Emulación de control remoto para Barra de Sonido Samsung.
* **Multiplexación de Variables:** Estrategia de software para evadir el límite de dispositivos gratuitos del broker. Se abstrajeron funciones complejas (secuencias de patrones luminosos y ecualización de bajos) enrutándolas sobre controles multimedia nativos (`Next`/`Previous`) mediante Rutinas de Alexa.
* **Acondicionamiento de Potencia:** Diseño con transistor NPN de conmutación (BC547) para maximizar la corriente del diodo emisor IR (940nm) aislando los puertos lógicos de 3.3V del ESP32.

## 🗂️ Estructura del Repositorio
* `/codigoF.ino` : Firmware principal en C++ para el ESP32 (Lógica MQTT, Interrupciones y Emisión IR).
* `/Backend_AWS_Lambda/` : Código fuente en Python con la implementación de la API REST para el control del sistema a través de Custom Skills en la nube de AWS (Capa de abstracción alternativa).

## 🛠️ Tecnologías y Hardware
* Microcontrolador ESP32-S3
* Diodo LED IR (38 kHz) + Transistor BC547
* Arduino IDE (Librería `IRremoteESP8266` y `SinricPro`)
* Amazon Alexa & Sinric Pro (IoT Middleware)
