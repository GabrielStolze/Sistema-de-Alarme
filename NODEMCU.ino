#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define INTERVALO_ENVIO       1500
#define DEBUG

//informações da rede WIFI
const char* ssid = "TED";                 //SSID da rede WIFI
const char* password =  "amor0525";    //senha da rede wifi
 
//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "hairdresser.cloudmqtt.com";   //server
const char* mqttUser = "efszlpdd";              //user
const char* mqttPassword = "lzNMDV9wrW1U";      //password
const int mqttPort = 15504 ;                     //port
const char* mqttTopicSub ="presenca/pir";            //tópico que sera assinado

int ultimoEnvioMQTT = 0;
char pir;

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao WiFi..");
    #endif
  }
  #ifdef DEBUG
  Serial.println("Conectado na rede WiFi");
  #endif
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
 
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
 
    }
  }

 //subscreve no tópico
  client.subscribe(mqttTopicSub);
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);
 
  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif
}
 
//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP8266Client", mqttUser, mqttPassword) :
                     client.connect("ESP8266Client");

    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado!");
      #endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub, 1); //nivel de qualidade: QoS 1
    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      //Aguarda 10 segundos 
      delay(10000);
    }
  }
}
 
void loop() {
  if (Serial.available() > 0)
  {
   pir = Serial.read(); 
  }
  
  if (!client.connected()) {
    reconect();
  }

  //envia a cada X segundos
  if ((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO)
  {
      envia();
      ultimoEnvioMQTT = millis();
  }
  client.loop();
}

//função para leitura do DHT11
void envia(){


  char MsgPirMQTT[10];
  
  if( pir == '1'){
    sprintf(MsgPirMQTT,"%c",pir);
    client.publish("presenca/pir", MsgPirMQTT);
  }if ( pir == '0'){
    sprintf(MsgPirMQTT,"%c",pir);
    client.publish("presenca/pir", MsgPirMQTT);
}}
