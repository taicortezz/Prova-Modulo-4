#include <WiFi.h>
#include <HTTPClient.h>

//#define led_azul 9 // Pino utilizado para controle do led azul
#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int pinoBotao = 18;  // O botão está no pino 18
int statusBotao = 0;  // variável para ler o status do botão
const int ldrPino = 4;  // ldr está no pino 4
int threshold= 600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(pinoBotao, INPUT); // Inicializa o botão com o input
//para que todos os leds fiquem apagados na inicialização
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW); 

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  statusBotao = digitalRead(pinoBotao);
  if (statusBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int codigoRespHTTP = http.GET(); // Código do Resultado da Requisição HTTP

    if (codigoRespHTTP>0) {
      Serial.print("Código da resposta HTTP: ");
      Serial.println(codigoRespHTTP);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Erro no código: ");
      Serial.println(codigoRespHTTP);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPino);

  if(ldrstatus<=threshold){ //Quando estiver escuro
    Serial.print("seu led escuro acende");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH); // pisca o led amarelo 
    delay(1000); // a cada 1 segundo


  }else{ //Se não estiver escuro, ou seja, quando estiver claro
    Serial.print("sua luz brilhante apaga");
    Serial.println(ldrstatus); 
    digitalWrite(led_verde, HIGH); //liga o led verde
    delay(3000);// por 3 segundos
    digitalWrite(led_verde, LOW); //desliga o led verde
    digitalWrite(led_amarelo, HIGH); //liga o led amarelo
    delay(2000); //por 2 segundos
    digitalWrite(led_amarelo, LOW); //desliga o led amarelo
    digitalWrite(led_vermelho, HIGH); //liga o led vermelho
    delay(5000); // por 5 segundos
    digitalWrite(led_vermelho, LOW); //desliga o led vermelho

  }

}