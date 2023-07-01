#include "roombaDefines.h"
#include <SoftwareSerial.h>

// Roomba 650 connection
int rxPin=10;
int txPin=11;
SoftwareSerial Roomba(rxPin,txPin);

// BT Module (HC-06)
SoftwareSerial BT1(8, 9); // pino 8 é Rx, pino 9 é Tx

void setup()
{
  Roomba.begin(19200);
  BT1.begin(19200);
  Serial.begin(19200);
  
  pinMode(ddPin, OUTPUT);
  pinMode(ledPin, OUTPUT); //spare if needed for test purpose
  pinMode(buttonPin, INPUT_PULLUP); //used for "start" for test purposes

  delay(2000);
  Serial.print("Roomba Remote Control...");
  BT1.print("Roomba Remote Control...");
  BT1.println('\n');
  
  wakeUp ();   // Wake-up Roomba
  startSafe(); // Start Roomba in Safe Mode

   while (digitalRead(buttonPin)) { } // wait button to be pressed to continous run code

   turnCW (40, 180); //testa se o roomba ira rodar no sentido horário 180 e parar. Ajuste o delay da funcao turnCW até que isso ocorra
   
}

//---------------------------------------------
void loop()
{

}

// Funções de Controle do roomba

void wakeUp (void)
{
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}
//---------------------------------------------
void startSafe()
{
  Roomba.write(128); //Start
  Roomba.write(131); //Safe mode
  delay(1000);
}
//---------------------------------------------
void setPowerLED(byte setColor, byte setIntensity)
{  
   color = setColor;
   intensity = setIntensity;
   Roomba.write(139);
   Roomba.write((byte)0x00);
   Roomba.write((byte)color);
   Roomba.write((byte)intensity);
}
//---------------------------------------------

void setDigitLEDFromASCII(byte digit, char letter)
{
   
   switch (digit)
   {
      case 1:
          digit1 = letter;
          break;
      case 2:
          digit2 = letter;
          break;
      case 3:
          digit3 = letter;
          break;
      case 4:
          digit4 = letter;
          break;
   }
   Roomba.write(164);
   Roomba.write(digit1);
   Roomba.write(digit2);
   Roomba.write(digit3);
   Roomba.write(digit4);
}
//---------------------------------------------

void writeLEDs (char a, char b, char c, char d)
// Exemplo: writeLEDs ( 's', 't', 'o', 'p');
{
   setDigitLEDFromASCII(1, a);
   setDigitLEDFromASCII(2, b);
   setDigitLEDFromASCII(3, c);
   setDigitLEDFromASCII(4, d);
}
//---------------------------------------------

//Movimentacao dos motores


void turnCW(unsigned short velocity, unsigned short degrees)
{
   drive(velocity, -1);
   clamp(velocity, 0, 500);
   delay(6600); // ajuste este delay conforme a velocidade para correção do angulo
   drive(0,0);
}
//---------------------------------------------------------------

void drive(int velocity, int radius)
{
   clamp(velocity, -500, 500); //def max and min velocity in mm/s
   clamp(radius, -2000, 2000); //def max and min radius in mm

   Roomba.write(137);
   Roomba.write(velocity >> 8);
   Roomba.write(velocity);
   Roomba.write(radius >> 8);
   Roomba.write(radius);
}

//---------------------------------------------------------------

void driveWheels(int right, int left)
{
   clamp(right, -500, 500);
   clamp(left, -500, 500);

   Roomba.write(145);
   Roomba.write(right >> 8);
   Roomba.write(right);
   Roomba.write(left >> 8);
   Roomba.write(left);
}

//---------------------------------------------------------------
void driveWheelsPWM(int rightPWM, int leftPWM)
{
   clamp(rightPWM, -255, 255);
   clamp(leftPWM, -255, 255);

   Roomba.write(146);
   Roomba.write(rightPWM >> 8);
   Roomba.write(rightPWM);
   Roomba.write(leftPWM >> 8);
   Roomba.write(leftPWM);
}

void turnCCW(unsigned short velocity, unsigned short degrees)
{
   drive(velocity, 1);
   clamp(velocity, 0, 500);
   delay(6600); // ajuste este delay conforme a velocidade para correção do angulo
   drive(0,0);
}

//---------------------------------------------------------------
void driveStop(void)
{
   drive(0,0);
}

//---------------------------------------------------------------
void driveLeft(int left)
{
   driveWheels(left, 0);
}

//---------------------------------------------------------------
void driveRight(int right)
{
   driveWheels(0, right);
}
//---------------------------------------------------------------




