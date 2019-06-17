#include "Seeed_BME280.h" // Bilbioteca do barometro.
#include "RTClib.h" // Biblioteca do relogio.
#include <Wire.h>

//test git

//Instanciando os objetos.
BME280 bme280;
RTC_DS3231 rtc;

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

int contador = 0; // Variavel do Contador

// Variaveis Temperatura.
double temperatura1 = 0;
double temperatura2 = 0;
double temperatura3 = 0;

// Variaveis Pressao.
double pressure;
double pressao1 = 0;
double pressao2 = 0;
double pressao3 = 0;

// Variaveis Tendencia.
char tendencia2temp = ' ';
char tendencia3temp = ' ';

void setup()
{
  
  Serial.begin(9600); // Seta velocidade da porta serial.
  
  //Check do sensor Pressao
  if(!bme280.init()){
    Serial.println("Device error!");
  }

  //Check do relogio
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  
  // Para ajsutar o relogio
  ///if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    //Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2019, 6, 16, 22, 14, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  ///}

  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
  
}

void loop()
{
  DateTime now = rtc.now();
  
  Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
  Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
  Serial.print(" "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
  Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
  Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
  Serial.print(" "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
  Serial.println(); //QUEBRA DE LINHA NA SERIAL
  Serial.println(); //QUEBRA DE LINHA NA SERIAL
  
  //get and print temperatures
  Serial.print("Temp Atual: ");
  Serial.print(bme280.getTemperature());
  Serial.println(" C");//The unit for  Celsius because original arduino don't support special symbols
  
  //get and print atmospheric pressure data
  Serial.print("Pres Atual: ");
  //Serial.print(pressure = bme280.getPressure());
  //Serial.println("Pa");
  Serial.print(pressure = bme280.getPressure()/100);
  Serial.println(" mbar");
  
  //get and print altitude data
  //Serial.print("Altitude: ");
  //Serial.print(bme280.calcAltitude(pressure));
  //Serial.println("m");

  //get and print humidity data
  Serial.print("Humid Atual: ");
  Serial.print(bme280.getHumidity());
  Serial.println(" %");
  
  contador++; //Incrementa o contador
  
    //Mostra o valor do contador no display
    //lcd.setCursor(10, 1);
    //lcd.print(contador);
    //Mostra o valor do contador no serial monitor
    
  // Saida para os dados seriais.
  Serial.print("Contador: ");
  Serial.println(contador);
     
  Serial.print("5m ");
  Serial.println(temperatura1);    
  
  Serial.print("15m ");
  Serial.print(temperatura2);
  Serial.println(tendencia2);   
  
  Serial.print("30m ");
  Serial.println(temperatura3);
  Serial.println(tendencia3);     
  
  Serial.println("------------------------");

  if(contador==6){
    temperatura1=bme280.getTemperature();
    //Serial.println("5m ");
    //Serial.print(temperatura1);
    }
    else if (contador==12) {
      temperatura2=bme280.getTemperature();
       
      if (temperatura2 > temperatura1) {
        tendencia2temp = '+';
        }
        
      if (temperatura2 < temperatura1) {
        tendencia2temp = '-';
        }  
         
      if (temperatura2 = temperatura1) {
        tendencia2temp = '=';
        }  
          
    }
      else if (contador==72) {
        temperatura3=bme280.getTemperature();
        
        if (temperatura3 > temperatura2) {
          tendencia3temp = '+';
        }
        
        if (temperatura3 < temperatura2) {
          tendencia3temp = '-';
        }  
         
        if (temperatura3 = temperatura2) {
          tendencia3temp = '=';
        }  

        contador = 0;
        }
    
  delay(5000);
}
