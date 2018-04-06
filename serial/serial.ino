//#define LED 13
void setup()
{
  Serial.begin(230400);
  //pinMode(LED, OUTPUT); 
  //Serial.write('A');
}

//bool state = false;

void loop()
{
  if(Serial.available())
    Serial.write(Serial.read());
  /*if(Serial.available())
  {
    Serial.write(((Serial.read() + 1) % 26) + 65);
    state = !state;
    if(state)
      digitalWrite(LED, HIGH);
    else
      digitalWrite(LED, LOW);
    delay(500);
  }*/
}
