#include "Seeed_BME280.h" // Bilbioteca do barometro.
#include "RTClib.h" // Biblioteca do relogio.
#include <Wire.h>
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*

//Instanciando os objetos.
BME280 bme280;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article

//DECLARAÇÃO variaveis
char daysOfTheWeek[7][12] = {"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};

int contador = 0; // Variavel do Contador

//Variaveis de Historico.
int TemperaturaArray[3][2];
int HumidadeArray[3][2];
int PressaoArray[3][2];

//Variavel do intervalo entre dados historiocos.
int intervalo[3] = {3, 6, 12};


String Hora;
String Data;
int pressure;
double temperaturaFull;
int UmidadeFull;
int temperatura;
int humidade;
String QualidadeAr;

boolean PrimeiraVez = true;
String Atual[3];

// Variaveis Tendencia.
//char tendencia2temp = ' ';
//char tendencia3temp = ' ';
//char tendencia2um = ' ';
//char tendencia3um = ' ';

void setup()
{
  Serial.begin(9600); // Seta velocidade da porta serial.
  
  // Set off LCD module
  lcd.begin (20,4); // 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  //lcd.setBacklight(LOW);
  
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
  //Leitura dos sensores.
  DateTime now = rtc.now();
  Hora = String(now.hour()) + ":" + String(now.minute()) + " ";
  Data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " " + String(daysOfTheWeek[now.dayOfTheWeek()]); 
  temperaturaFull=bme280.getTemperature();
  UmidadeFull=bme280.getHumidity();
  pressure = bme280.getPressure()/100;
    
  //Verifica a qualidade do ar.
  if (UmidadeFull>29) {
    QualidadeAr=("Umidade OK");
    }
    else{
      if (UmidadeFull>20){
        QualidadeAr=("Umid. Atencao");
        }
        else{
          if (UmidadeFull>11){
            QualidadeAr=("Umid. Alerta");
            }
            else{
              QualidadeAr=("Umid. Emergen");
              }
            }
      }
  
  //Primeiro ponto de historico.
  if(contador==intervalo[0]){

    //Le os sensores
    TemperaturaArray[0][0]=temperaturaFull;
    HumidadeArray[0][0]=UmidadeFull;
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
    TemperaturaArray[1][0]=temperaturaFull;
    HumidadeArray[1][0]=UmidadeFull;
    PressaoArray[1][0]=pressure;

    //Calcula a diferenca
    TemperaturaArray[1][1]=TemperaturaArray[1][0] - TemperaturaArray[0][0];
    HumidadeArray[1][1]=HumidadeArray[1][0] - HumidadeArray[0][0];
    PressaoArray[1][1]=PressaoArray[1][0] - PressaoArray[0][0];
    
    //Marca qual a ultima leitura
    Atual[0]=" ";
    Atual[1]="*";
    Atual[2]=" ";
   }  
  
  // Terceiro ponto do historico.
  if (contador==intervalo[2]) {
    //Le os sensores
    TemperaturaArray[2][0]=temperaturaFull;
    HumidadeArray[2][0]=UmidadeFull;
    PressaoArray[2][0]=pressure;

     //Calcula a diferenca
    TemperaturaArray[2][1]=TemperaturaArray[2][0] - TemperaturaArray[1][0];
    HumidadeArray[2][1]=HumidadeArray[2][0] - HumidadeArray[1][0];
    PressaoArray[2][1]=PressaoArray[2][0] - PressaoArray[1][0];

    //Marca qual a ultima leitura
    Atual[0]=" ";
    Atual[1]=" ";
    Atual[2]="*";
                  
    contador = 0;
    }  
  
  /// Exibe no Display.
  
  //Data e hora.
  lcd.setCursor(0,0);
  lcd.print(Hora);
  lcd.print(" ");
  lcd.print(Data);
  lcd.setCursor(0,1);
  
  //Temperatura.
  lcd.print("T ");
  lcd.setCursor(2,1);
  lcd.print(temperaturaFull);
  lcd.setCursor(7,1);
  lcd.print("             ");
  lcd.setCursor(8,1);
  lcd.print(Atual[0]);
  lcd.print(TemperaturaArray[0][1]);
  lcd.print(" ");
  lcd.print(Atual[1]);
  lcd.print(TemperaturaArray[1][1]);
  lcd.print(" ");
  lcd.print(Atual[2]);
  lcd.print(TemperaturaArray[2][1]);

  //Umidade.
  lcd.setCursor(0,2);
  lcd.print("U ");
  lcd.setCursor(2,2);
  lcd.print(UmidadeFull);
  lcd.print("% ");
  lcd.setCursor(6,2);
  lcd.print("             ");
  lcd.setCursor(6,2);
  lcd.print(QualidadeAr);

  //Pressão
  lcd.setCursor(0,3);
  lcd.print("P ");
  lcd.setCursor(2,3);
  lcd.print(pressure);
  lcd.setCursor(6,3);
  lcd.print("             ");
  lcd.setCursor(6,3);
  lcd.print(Atual[0]);
  lcd.print(PressaoArray[0][1]);
  lcd.print(" ");
  lcd.print(Atual[1]);
  lcd.print(PressaoArray[1][1]);
  lcd.print(" ");
  lcd.print(Atual[2]);
  lcd.print(PressaoArray[2][1]);
    
  contador++; //Incrementa o contador
      
  delay(5000);
}
