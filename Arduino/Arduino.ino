/*Código Arduino para integração com aplicativo Android criado na plataforma App Inventor
 * para integração com Middleware para Internet das Coisas usando sensores como Led, Buzzer e Senso Motor
 * 
 * Elaborado por Lhaís Rodrigues, Programação Concorrente e Distribuída - CIn UFPE
*/

#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

//byte mac[] = { 60, 6D, C7, 82, D4, 35 }; //Endereço Mac => Obs.: Confirmar o uso deste MAC
byte ip[] = { 192, 168, 93, 1 }; //Endereço de IP da sua Rede: Retirado do ipconfig -all
EthernetServer server(8090); //Porta do Serviço

int led = 13; //Pino onde o led está conectado
int buzzer = 10; //Pino onde o buzzer está conectado
String readString = String(30); //String para buscar dados de endereço
boolean statusLed = false; //Variável para o status do led 
boolean statusBuzzer = false; //Variável para o status do buzzer

void setup(){
  //Ethernet.begin(mac, ip);  //Inicia o Shield Ethernet => Obs.: Verificar o MAC
  pinMode(led, OUTPUT); //Definindo o pino ethernet como saída
  pinMode(buzzer, OUTPUT); //Definindo o pino buzzer como saída
  Serial.begin(9600);  //Inicia a comunicação Serial
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
            delay(500);
            statusBuzzer = true;
          }
          if(readString.indexOf("DesligarBuzzer") >= 0){
            //O Buzzer vai ser desligado
            noTone(buzzer);
            delay(500);
            statusBuzzer = false;
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
        
        //Limpa string para a próxima leitura
        readString="";
        
        //Parar a conexão cliente
        client.stop();
        }
      }
    }
  }
}
