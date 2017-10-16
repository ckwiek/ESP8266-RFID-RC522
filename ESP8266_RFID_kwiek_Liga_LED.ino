/*
 *********************************************************************************************************
 ESTE CODIGO TEM A INTENÇÃO DE MOSTRAR UMA APLICAÇÃO USANDO UM ESP8266-12,
 E UM SHIELD RFID-RC522, NA PRIMEIRA VEZ APOS CARREGAR O CODIGO, ABRA O MONITOR SERIAL,
 CONFIGURE O MESMO PARA A VELOCIDADE DE 9600 BAUD RATE,E PRESSIONE O BOTÃO RESET DA SUA PLACA,
 NO MONITOR SERIAL VOCÊ IRA OBSERVAR A MENSAGEM: APROXIME SEU CARTAO (TAG).
 AO APROXIMAR SEU CARTAO,COMO ELE AINDA NAO ESTA CADASTRADO ELE NÃO ACIONARA O GPIO,
 MAS IMPRIMIRA A TAG DO MESMO, DESTA FORMA VOCE AGORA PODE COLOCAR NO CODIGO A TAG DO
 SEU CARTÃO, OU CHAVEIRO, INSIRA A TAG NA LINHA: 
 if (conteudo.substring(1) == "AQUI SUA TAG" || "AQUI OUTRA SE QUISER") //UID 1 - 2....
 E CARREGUE O CODIGO COM SUA TAG, OU TAGS INSERIDAS, AGORA DEPOIS DE CARREGAR E PRESSIONAR
 O BOTÃO RESET, APROXIME O CARTÃO, E O LED ACENDERA POR 3 SEGUNDOS, VOCÊ PODE DEFINIR ESSE
 TEMPO COMO QUISER, NESTA SAIDA PODE USAR UM RELE PARA ABRIR UMA FECHADURA POR EXEMPLO.

 ESTE CODIGO FONTE É LIVRE E SEGUE OS PRINCIPIOS GNU, PODE SER ADAPTADO, NODIFICADO
 PARCIAL OU INTEIRAMENTE SEM PERVIO AVISO POR QUALQUER PESSOA.

 COMIGO FUNCIONOU PERFEITAMENTE SEM PROBLEMA ALGUM, MAS NÃO GARANTO SEU FUNCIONAMENTO,
 FICA POR CIONTA E RISCO DE QUEM QUISER USAR, DUVIDAS FAZER ME CONTATAR NO E-MAIL ABAIXO.

 contato@carloskwiek.com.br 

 Carlos kwiek, engenheiro eletronico amante de eletronica e programação.
 www.carloskwiek.com.br 
 ************************************************************************************************************
 *************************PINAGEM****************************************************************************
  
  RFID-RC522   Wemos          Nodemcu
RST/Reset RST  D3  [1*]        D3 [1*]       GPIO-0  [1*]
SPI SS  SDA[3] D8  [2*]        D8 [2*]       GPIO-15 [2*]
SPI MOSI MOSI  D7             D7           GPIO-13
SPI MISO MISO  D6             D6           GPIO-12
SPI SCK SCK    D5             D5           GPIO-14

[1*] (1, 2) Configuracao tipica definindo como RST_PIN no sketch / programa.
[2*] (1, 2) Configurable, tipica definindo como SS_PIN no sketch / programa.
[3*] O pino SDA pode ser rotulado como SS em algumas placas de MFRC522 / mais antigas, significam a mesma coisa.
******************************************************************************************************************
 */


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    D3    
#define SS_PIN     D8   
#define LED        D1

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  digitalWrite(D1,HIGH);
  pinMode(D1, OUTPUT);
 
}

void loop() 
{
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6") //UID 1 - Cartao
  {
    Serial.println("Liberado !");
    Serial.println();
    digitalWrite(D1, LOW);     // LIGA LED OU/ ativa rele, abre trava solenoide
    delay(3000);              // DELAY /espera 3 segundos
    digitalWrite(D1, HIGH);  // DESlIGA LED OU /desativa rele, fecha  trava solenoide
  }  
}
