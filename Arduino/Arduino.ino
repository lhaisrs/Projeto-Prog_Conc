/*Código Arduino para integração com aplicativo Android criado na plataforma App Inventor
 * para integração com Middleware para Internet das Coisas usando sensores como Led, Buzzer e Senso Motor
 * 
 * Elaborado por Lhaís Rodrigues, Programação Concorrente e Distribuída - CIn UFPE
*/

#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
#include <Servo.h>


byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; //Endereço Mac
byte ip[] = { 192, 168, 0, 101 }; //Endereço de IP da sua Rede: Retirado do ipconfig -all
EthernetServer server(8090); //Porta do Serviço

Servo motor; //Criamos a variável do tipo Servo
int led = 13; //Pino onde o led de alto brilho está conectado
int buzzer = 12; //Pino onde o buzzer está conectado
int led_motor = 11; //Pino onde o led auxiliar do sensor motor está conectado
int senso_motor = 3; //Pino onde o sensor metor está conectado
String readString = String(30); //String para buscar dados de endereço
boolean statusLed = false; //Variável para o status do led 
boolean statusBuzzer = false; //Variável para o status do buzzer
boolean statusMotor = false; //Variável para o status do motor

void setup(){
  Ethernet.begin(mac, ip);  //Inicia o Shield Ethernet => Obs.: Verificar o MAC
  server.begin();
  pinMode(led, OUTPUT); //Definindo o pino ethernet como saída
  pinMode(buzzer, OUTPUT); //Definindo o pino buzzer como saída
  pinMode(led_motor, OUTPUT); //Definindo o pino do led auxiliar do senso motor
  Serial.begin(9600);  //Inicia a comunicação Serial
  motor.attach(senso_motor);
}

void loop(){
  // Criar uma conexão de cliente
  EthernetClient client = server.available();
  
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        // ler caractere por caractere vindo do HTTP
        if (readString.length() < 30)
        {
          // armazena os caracteres para string
          readString += (c);
        }
        
        //se o pedido HTTP terminou
        if (c == '\n')
          {
          //vamos verificar se o LED deve ser ligado
          //Se a string possui o texto Ligar, ele deve ligar
          if(readString.indexOf("LigarLed") >= 0)
          {
            //O Led vai ser ligado
            digitalWrite(led, HIGH);
            statusLed = true;
          }
          //Se a string possui o texto Desligar, ele deve desligar
          if(readString.indexOf("DesligarLed") >= 0)
          {
            //O Led vai ser desligado
            digitalWrite(led, LOW);
            statusLed = false;
          }
          if(readString.indexOf("LigarBuzzer") >= 0){
            //O Buzzer vai ser ligado
            tone(buzzer, 1500);
            statusBuzzer = true;
          }
          if(readString.indexOf("DesligarBuzzer") >= 0){
            //O Buzzer vai ser desligado
            noTone(buzzer);
            statusBuzzer = false;
          }

          //Tratando o servo motor
          //Usamos          
          if(readString.indexOf("LigarMotor") >= 0){
            //O Sensor Motor vai ser ligado
            int i; 
            for(i = 0; i < 90; i++){
              motor.write(i);
            }

            delay(1000);
            
            if(i == 90){
              digitalWrite(led_motor, HIGH);
            }

            statusMotor = true;
          }

          if(readString.indexOf("DesligarMotor") >= 0){
            //O Sensor Motor vai ser desligado
            int i; 
            for(i = 90; i > 0; i--){
              motor.write(i);
            }
            if(i == 0){
              digitalWrite(led_motor, LOW);
            }

            statusMotor = false;
          }

        //Dados HTML de saída começando com cabeçalho padrão
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();

        //Estilizando a saída HTML
        client.print("<font size='20'>");
        client.print("Status Led: ");
        client.print("<font size='20'>");
        if (statusLed) {
          client.print("LedLigado");
        } else {
          client.print("LedDesligado");
        }
        
        client.print("<font size='20'>");
        client.print("Status Buzzer: ");
        if (statusBuzzer) {
          client.print("BuzzerLigado");
        } else {
          client.print("BuzzerDesligado");
        }

        client.print("<font size='20'>");
        client.print("Status Senso Motor: ");
        if (statusMotor) {
          client.print("SensoMotorLigado");
        } else {
          client.print("SensorMotorDesligado");
        }
        
        //Limpa string para a próxima leitura
        readString="";
        
        //Parar a conexão cliente
        client.stop();
        }
      }
    }
  }
}
