#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>
#define pino_trigger 4
#define pino_echo 5

int sensor = A0;
int valorSensor = 0; //Usada para ler o valor do sensor em tempo real.
float cmMsec, distM;
Ultrasonic ultrasonic(pino_trigger, pino_echo);
const int rele = 10;
boolean ativarFunViagem;
EthernetServer server = EthernetServer(1000);
int led = 7;
boolean verifica;


void setup() {
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(sensor, INPUT);

  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  IPAddress myIP(192, 168, 0, 6);

  Serial.println("Iniciando...");

  Ethernet.begin(mac, myIP);

  server.begin();

  Serial.println("Rodando!!!");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  verifica = false;
  ativarFunViagem = false;
}

void loop() {
  EthernetClient client = server.available();
  if (client.connected())
  {
    Serial.println("Cliente conectado!");
    if (client.available()) {
      char c = client.read();
      Serial.println(c);
      if (c == '1') {
        if (verifica == false) {
          digitalWrite(led, HIGH);
          verifica = true;
          Serial.println("Lâmpada ligada");
        } else if (verifica == true) {
          digitalWrite(led, LOW);
          verifica = false;
          Serial.println("Lâmpada desligada");
        }
        client.println("55555");
      } else if (c == '2') { //Funcão Viagem
          if (ativarFunViagem == false) {
            ativarFunViagem = true;
            Serial.println("Função viagem Ativada");
            client.println("666666");
          } else if (ativarFunViagem == true) {
            ativarFunViagem = false;
            Serial.println("Função viagem Ativada");
            client.println("77777");
          }
      } //else {
        //Serial.println("Nada aconteceu!");
      //}
    }
  }

  if(ativarFunViagem == true){
    Serial.println("Chegou Aqui");    
    leituraLuminosidade();
  }


  client.stop();
  delay(1000);

}


void leituraLuminosidade() {
 
  int valorSensor = analogRead(sensor);
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  distM = cmMsec / 100;

  if (valorSensor >= 990) {
    if (distM <= 1) {
      digitalWrite(rele, HIGH);
      delay(10000);
    }
  } else {
    delay(3000);
    digitalWrite(rele, LOW);

  }
  Serial.println(valorSensor);

  delay(50);
  

}


