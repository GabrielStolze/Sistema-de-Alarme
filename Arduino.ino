#include "Servo.h" 
#include <Wire.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte linhas = 4; 
const byte colunas = 4;

char matrizteclado[linhas][colunas] = {
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinoslinhas[linhas] = {9,8,7,6}; //pinos utilizados nas linhas
byte pinoscolunas[colunas] = {5,4,3,2}; //pinos utilizados nas colunas

char* senha = "1234"; 

int LedVermelho = 13; 
int buzzer = 11; 
int sensorPir = 10;  

int variavel_Senha_armazenada = 0 , leitura = 0;
char digito; 
 
Keypad teclado = Keypad( makeKeymap(matrizteclado), pinoslinhas, pinoscolunas, linhas, colunas );
LiquidCrystal_I2C lcd(0x27,16,2);  // Criando um LCD de 16x2 no endereço 0x20
Servo meuservo; 

void setup()
{
  lcd.init();                 // Inicializando o LCD
  lcd.backlight();            // Ligando o BackLight do LCD
  lcd.setCursor(5,0);
  lcd.print("SISTEMA"); 
  lcd.setCursor(3,1);
  lcd.print("DESATIVADO");
 
  meuservo.attach(12);
  meuservo.write(100);
  Serial.begin(115200);

  pinMode(LedVermelho,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(sensorPir,INPUT);
}

void loop(){

leitura = digitalRead(sensorPir);
digito = teclado.getKey(); 
Serial.write("0");


if(leitura == 1){
digitalWrite(LedVermelho,HIGH);
digitalWrite(buzzer,HIGH);
meuservo.write(0);

Serial.write("1");

delay(100);
lcd.clear();
lcd.setCursor(5,0);
lcd.print("SISTEMA"); 
lcd.setCursor(5,1);
lcd.print("ATIVADO");
}

if (digito !=0){

if (digito == senha[variavel_Senha_armazenada]) variavel_Senha_armazenada ++;
else variavel_Senha_armazenada = 0;

if (variavel_Senha_armazenada == 4){
variavel_Senha_armazenada = 0;

digitalWrite(LedVermelho,LOW);  
digitalWrite(buzzer,LOW); 

lcd.clear();
lcd.setCursor(5,0);
lcd.print("SISTEMA"); 
lcd.setCursor(3,1);
lcd.print("DESATIVADO");
meuservo.write(100);
Serial.write("0");
    }
  } 
}
