#include "Seeed_BME280.h" // Bilbioteca do barometro.
#include "RTClib.h" // Biblioteca do relogio.
#include <Wire.h>

//Instanciando os objetos.
BME280 bme280;
RTC_DS3231 rtc;

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

int contador = 0; // Variavel do Contador

//Variavel Historico Temperatura.
double TemperaturaArray[3][2];

//Variavel do intervalo entre dados historiocos.
int intervalo[2] = {3, 6, 9};

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
	else{
		Serial.println("Barometro e Termometro - OK");
		}

  //Check do relogio
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
	  else{
	  	Serial.println("Relogio - OK");
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
  
	//Monta e imprimi Hora e Data
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
  Serial.print(pressure = bme280.getPressure()/100);
  Serial.println(" mbar");
  
  // Se quiser pegar a altitude pela pressao.
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
  Serial.print(TemperaturaArray[0][0]);
  Serial.print(" ");
  Serial.println(TemperaturaArray[0][1]);
  
  Serial.print("15m ");
  Serial.print(temperatura2);
  Serial.println(tendencia2);   
  
  Serial.print("30m ");
  Serial.println(temperatura3);
  Serial.println(tendencia3);     
  
  Serial.println("------------------------");

  
  //Primeiro ponto de historico.
  if(contador==intervalo[0]){
    TemperaturaArray[0][0]=bme280.getTemperature();
    TemperaturaArray[0][1]=now.hour();
    }

  //Segundo ponto do historico.
  if (contador==intervalo[1]) {
    TemperaturaArray[1][0]=bme280.getTemperature();
    TemperaturaArray[1][1]=now.hour();
    
    if (TemperaturaArray[1][0] = TemperaturaArray[0][0]) {
        tendencia2temp = '=';
        }
        
        else{
          if (TemperaturaArray[1][0] > TemperaturaArray[0][0]) {
          tendencia2temp = '+';
          }
          
            else{
              tendencia2temp = '-';
              }
        }
    }  
  
  // Terceiro ponto do historico.
  if (contador==intervalo[2]) {
	   TemperaturaArray[2][0]=bme280.getTemperature();
     TemperaturaArray[2][1]=now.hour();
     
    
    if (TemperaturaArray[2][0] = TemperaturaArray[1][0]) {
		tendencia3temp = '=';
        }
        
    else{
		if (TemperaturaArray[2][0] = TemperaturaArray[1][0]) {
            tendencia3temp = '+';
			}
          
        else{
			tendencia3temp = '-';
			}
        }
    
      contador = 0;
    }  
    
  delay(5000);
}
