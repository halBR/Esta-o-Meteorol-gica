#include "Seeed_BME280.h" // Bilbioteca do barometro.
#include "RTClib.h" // Biblioteca do relogio.
#include <Wire.h>

//Instanciando os objetos.
BME280 bme280;
RTC_DS3231 rtc;

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};

int contador = 0; // Variavel do Contador

//Variaveis de Historico.
int TemperaturaArray[3][2];
int HumidadeArray[3][2];
int PressaoArray[3][2];

//Variavel do intervalo entre dados historiocos.
int intervalo[3] = {3, 6, 12};

// Variaveis Pressao.
int pressure;
double temperaturaFull;
int temperatura;
int humidade;

// Variaveis Tendencia.
char tendencia2temp = ' ';
char tendencia3temp = ' ';

String Hora;
String Data;

boolean PrimeiraVez = true;

String Atual[3];

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

  Hora = String(now.hour()) + ":" + String(now.minute()) + " ";
  Data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " " + String(daysOfTheWeek[now.dayOfTheWeek()]); 
  
  Serial.print(Hora);// + ':' + Minuto); //QUEBRA DE LINHA NA SERIAL
  Serial.println(Data);// + ':' + Minuto); //QUEBRA DE LINHA NA SERIAL
  
  //get and print temperatures
  Serial.print("Temp Atual: ");
  temperaturaFull=bme280.getTemperature();
  Serial.print(temperaturaFull);
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

  Serial.print(Atual[0]);   
  Serial.print("1x ");
  Serial.print (TemperaturaArray[0][0]);
  Serial.print(" ");
  Serial.print(TemperaturaArray[0][1]);
  Serial.print(" ");
  Serial.print(HumidadeArray[0][0]);
  Serial.print(" ");
  Serial.println(PressaoArray[0][0]);
  
  Serial.print(Atual[1]); 
  Serial.print("2x ");
  Serial.print(TemperaturaArray[1][0]);
  Serial.print(" ");
  Serial.print(tendencia2temp);  
  Serial.print(TemperaturaArray[1][1]);
  Serial.print(" ");
  Serial.print(HumidadeArray[1][0]);
  Serial.print(" ");
  Serial.println(PressaoArray[1][0]);
  
  Serial.print(Atual[2]); 
  Serial.print("3x ");
  Serial.print(TemperaturaArray[2][0]);
  Serial.print(" ");
  Serial.print(tendencia3temp);  
  Serial.print(TemperaturaArray[2][1]);
  Serial.print(" ");
  Serial.print(HumidadeArray[2][0]);
   Serial.print(" ");
  Serial.println(PressaoArray[2][0]);
  Serial.println("------------------------");
  
  //Primeiro ponto de historico.
  if(contador==intervalo[0]){
    
    //Le os sensores
    TemperaturaArray[0][0]=bme280.getTemperature();
    HumidadeArray[0][0]=bme280.getHumidity();
    PressaoArray[0][0]=pressure;
    
    //Marca qual a ultima leitura
    Atual[0]="*";
    Atual[1]=" ";
    Atual[2]=" ";
    
    if(PrimeiraVez){
        //Zera a diferença na primeira vez
        TemperaturaArray[0][1] = 0;
        HumidadeArray[0][1] = 0;
        PressaoArray[0][1] = 0;
        
        PrimeiraVez = false;
        
        }
        else{
			    ////Calcula a diferenca
			    TemperaturaArray[0][1] = TemperaturaArray[0][0] - TemperaturaArray[2][0];
          HumidadeArray[0][1] = HumidadeArray[0][0] - HumidadeArray[2][0];
          PressaoArray[0][1] = PressaoArray[0][0] - PressaoArray[2][0]; 
     
			}
    }

  //Segundo ponto do historico.
  if (contador==intervalo[1]) {
    //Le os sensores
    TemperaturaArray[1][0]=bme280.getTemperature();
    HumidadeArray[1][0]=bme280.getHumidity();
    PressaoArray[1][0]=pressure;

    //Calcula a diferenca
    TemperaturaArray[1][1]=TemperaturaArray[1][0] - TemperaturaArray[0][0];
    HumidadeArray[1][1]=HumidadeArray[1][0] - HumidadeArray[0][0];
    PressaoArray[1][1]=PressaoArray[1][0] - PressaoArray[0][0];
    
    //Marca qual a ultima leitura
    Atual[0]=" ";
    Atual[1]="*";
    Atual[2]=" ";
    
    if (TemperaturaArray[1][0] == TemperaturaArray[0][0]) {
        tendencia2temp = '=';
        }
        
        else{
			if (TemperaturaArray[1][0] > TemperaturaArray[0][0]) {
				tendencia2temp = '+';
				}
			}
    }  
  
  // Terceiro ponto do historico.
  if (contador==intervalo[2]) {
	  //Le os sensores
	  TemperaturaArray[2][0]=bme280.getTemperature();
    HumidadeArray[2][0]=bme280.getHumidity();
    PressaoArray[2][0]=pressure;

     //Calcula a diferenca
    TemperaturaArray[2][1]=TemperaturaArray[2][0] - TemperaturaArray[1][0];
    HumidadeArray[2][1]=HumidadeArray[2][0] - HumidadeArray[1][0];
    PressaoArray[2][1]=PressaoArray[2][0] - PressaoArray[1][0];

    //Marca qual a ultima leitura
    Atual[0]=" ";
    Atual[1]=" ";
    Atual[2]="*";
     
    if (TemperaturaArray[2][0] == TemperaturaArray[1][0]) {
		tendencia3temp = '=';
        }
        
    else{
		if (TemperaturaArray[2][0] > TemperaturaArray[1][0]) {
            tendencia3temp = '+';
			}
        }
    
      contador = 0;
    }  
    
  delay(5000);
}
