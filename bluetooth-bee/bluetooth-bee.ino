#define BOUD_RATE 38400
#define WAIT 1000

const String PRE_CMD("\r\n+");
const String POST_CMD("\r\n");

void setup() 
{ 
    Serial.begin(BOUD_RATE);
    initBT();
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

void initBT()
{
  Serial.println("BT initialization");
  Serial4.begin(BOUD_RATE);
  delay(WAIT);
  sendBT("STWMOD=0");
  sendBT("STNA=teensy");
  sendBT("STAUTO=0");
  sendBT("STOAUT=1");
  sendBT("STPIN=0000");
  sendBT(String("STBD=") + BOUD_RATE);
  sendBT("INQ=1");
  Serial.println("BT ready");
}
 
void sendBT(String cmd)
{
  Serial.println("CMD: " + cmd);
  cmd = PRE_CMD + cmd + POST_CMD;
  Serial4.print(cmd);
  delay(WAIT);
  while(Serial4.available())
  {
    Serial.write(Serial4.read());
  }
}
