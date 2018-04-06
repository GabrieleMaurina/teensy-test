
//#include <SoftwareSerial.h>   //Software Serial Port
#define blueToothSerial Serial4
#define RxD 31
#define TxD 32
 
#define DEBUG_ENABLED  1
 
//SoftwareSerial blueToothSerial(RxD,TxD);
 
void setup() 
{ 
    Serial.begin(38400);
    Serial.println("setup");
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection();
    Serial.println("setup exit");
}

char c; 
void loop() 
{
  Serial.println("qwer");
  delay(100);
  if(blueToothSerial.available())
  {
    c = blueToothSerial.read();
    Serial.write(c);
  }
  if(Serial.available())
  {
    c = Serial.read();
    blueToothSerial.write(c);
  }
} 
 
 
void setupBlueToothConnection()
{
    blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
    delay(8000);
    Serial.println("zxcv");
    sendBlueToothCommand("\r\n+STWMOD=0\r\n");
    sendBlueToothCommand("\r\n+STNA=teensy\r\n");
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");
    sendBlueToothCommand("\r\n +STPIN=0000\r\n");
    delay(5000); // This delay is required.
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    delay(5000); // This delay is required.
}

void CheckOK()
{
  char a,b;
  while(1)
  {
    if(blueToothSerial.available())
    {
      a = blueToothSerial.read();
      Serial.write(a);
      if('O' == a)
      {
        // Wait for next character K. available() is required in some cases, as K is not immediately available.
        while(blueToothSerial.available()) 
        {
           b = blueToothSerial.read();
           break;
        }
        Serial.write(b);
        if('K' == b)
        {
          Serial.println("ASDF");
          break;
        }
      }
   }
  }
  
  while( (a = blueToothSerial.read()) != -1)
  {
    //Wait until all other response chars are received
  }
}
 
void sendBlueToothCommand(char command[])
{
    blueToothSerial.print(command);
    CheckOK();   
}
