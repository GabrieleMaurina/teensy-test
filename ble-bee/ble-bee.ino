#define Rx 31
#define Tx 32
#define BOUD_RATE 9600
#define WAIT 8000

void setup() 
{ 
    Serial.begin(BOUD_RATE);
    pinMode(Rx, INPUT);
    pinMode(Tx, OUTPUT);
    setupBlueToothConnection();
}

void loop() 
{ 
  if(Serial4.available())
  {
    Serial.write(Serial4.read());
  }
  if(Serial.available())
  {
    Serial4.write(Serial.read());
  }
} 
 
void setupBlueToothConnection()
{
    Serial.print("Setting up Bluetooth link");
    Serial4.begin(BOUD_RATE);
    delay(WAIT);
    sendBlueToothCommand("AT");
    sendBlueToothCommand("AT");
    //sendBlueToothCommand("AT+CLEAR");
    sendBlueToothCommand("AT+TYPE?");
    sendBlueToothCommand("AT+TYPE0");
    sendBlueToothCommand("AT+CHAR0xFFE1");
    sendBlueToothCommand("AT+CHAR?");
    sendBlueToothCommand("AT+ADDR?");
    sendBlueToothCommand("AT+UUID0xFFE0");
    sendBlueToothCommand("AT+UUID?");
    sendBlueToothCommand("AT+HELP?");
    sendBlueToothCommand("AT+BAUD0");
    sendBlueToothCommand("AT+BAUD?");
    sendBlueToothCommand("AT+ROLE0");
    sendBlueToothCommand("AT+ROLE?");
    sendBlueToothCommand("AT+NAMETeensy");
    sendBlueToothCommand("AT+NAME?");
    sendBlueToothCommand("AT+PASS?");
    sendBlueToothCommand("AT+PIN123456");
    sendBlueToothCommand("AT+SAVE1");
    sendBlueToothCommand("AT+SAVE?");
    sendBlueToothCommand("AT+VERS?");
    Serial.print("Setup complete");
}
 
void sendBlueToothCommand(String command)
{
    Serial4.print(command);
    Serial.println(command);
    delay(WAIT);
    while(Serial4.available())
    {
       Serial.write(Serial4.read());
    }
    Serial.println("");
}
