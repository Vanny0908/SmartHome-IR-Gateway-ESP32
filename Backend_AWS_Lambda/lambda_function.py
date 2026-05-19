import json

def lambda_handler(event, context):
    request_type = event['request']['type']
    
    if request_type == "LaunchRequest":
        return construir_respuesta("Sistema domótico iniciado. ¿Qué equipo deseas controlar, Giovanny?", mantener_sesion=True)
    
    elif request_type == "IntentRequest":
        intent_name = event['request']['intent']['name']
        
        # --- LÓGICA DE LA TV ---
        if intent_name == "EncenderTvIntent":
            return construir_respuesta("Entendido, encendiendo el televisor.", mantener_sesion=True)
        elif intent_name == "ApagarTvIntent":
            return construir_respuesta("Apagando el televisor.", mantener_sesion=True)
        elif intent_name == "SubirVolumenTvIntent":
            return construir_respuesta("Subiendo el volumen de la tele.", mantener_sesion=True)
        elif intent_name == "BajarVolumenTvIntent":
            return construir_respuesta("Bajando el volumen de la tele.", mantener_sesion=True)
        elif intent_name == "SubirCanalTvIntent":
            return construir_respuesta("Subiendo de canal.", mantener_sesion=True)
        elif intent_name == "BajarCanalTvIntent":
            return construir_respuesta("Bajando de canal.", mantener_sesion=True)
            
        # --- LÓGICA DE LA BARRA DE SONIDO ---
        elif intent_name == "EncenderBarraIntent":
            return construir_respuesta("Prendiendo la barra de sonido.", mantener_sesion=True)
        elif intent_name == "ApagarBarraIntent":
            return construir_respuesta("Apagando la barra de sonido.", mantener_sesion=True)
        elif intent_name == "SubirVolumenBarraIntent":
            return construir_respuesta("Subiendo el volumen a la barra.", mantener_sesion=True)
        elif intent_name == "BajarVolumenBarraIntent":
            return construir_respuesta("Bajándole a la barra.", mantener_sesion=True)
        elif intent_name == "MuteBarraIntent":
            return construir_respuesta("Silenciando la barra de sonido.", mantener_sesion=True)
        elif intent_name == "SubirBajosIntent":
            return construir_respuesta("Aumentando los bajos.", mantener_sesion=True)
        elif intent_name == "BajarBajosIntent":
            return construir_respuesta("Reduciendo los bajos.", mantener_sesion=True)
        elif intent_name == "ResetBajosIntent":
            return construir_respuesta("Bajos reseteados a cero.", mantener_sesion=True)

        # --- LÓGICA DE LAS LUCES LED ---
        elif intent_name == "EncenderLucesIntent":
            return construir_respuesta("Iluminando el cuarto.", mantener_sesion=True)
        elif intent_name == "ApagarLucesIntent":
            return construir_respuesta("Apagando las luces.", mantener_sesion=True)
        elif intent_name == "LucesRojasIntent":
            return construir_respuesta("Cambiando las luces a color rojo.", mantener_sesion=True)
        elif intent_name == "LucesVerdesIntent":
            return construir_respuesta("Cambiando las luces a color verde.", mantener_sesion=True)
        elif intent_name == "LucesAzulesIntent":
            return construir_respuesta("Cambiando las luces a color azul.", mantener_sesion=True)
        elif intent_name == "PatronUnoIntent":
            return construir_respuesta("Activando el patrón uno.", mantener_sesion=True)
        elif intent_name == "PatronDosIntent":
            return construir_respuesta("Activando el patrón dos.", mantener_sesion=True)
        elif intent_name == "PatronTresIntent":
            return construir_respuesta("Activando el patrón tres.", mantener_sesion=True)
        elif intent_name == "PatronCuatroIntent":
            return construir_respuesta("Activando el patrón cuatro.", mantener_sesion=True)

        # --- LÓGICA DE LA ASPIRADORA ---
        elif intent_name == "EncenderAspiradoraIntent":
            return construir_respuesta("Iniciando la limpieza con la aspiradora.", mantener_sesion=True)
        elif intent_name == "ApagarAspiradoraIntent":
            return construir_respuesta("Deteniendo la aspiradora.", mantener_sesion=True)

        # --- SALIDA Y SEGURIDAD ---
        elif intent_name == "AMAZON.CancelIntent" or intent_name == "AMAZON.StopIntent":
            return construir_respuesta("Apagando el sistema domótico. Nos vemos.", mantener_sesion=False)
        else:
            return construir_respuesta(f"Recibí el comando {intent_name}, pero aún no tiene código.", mantener_sesion=True)

def construir_respuesta(texto_a_hablar, mantener_sesion=False):
    return {
        "version": "1.0",
        "response": {
            "outputSpeech": {
                "type": "PlainText",
                "text": texto_a_hablar
            },
            "shouldEndSession": not mantener_sesion 
        }
    }
