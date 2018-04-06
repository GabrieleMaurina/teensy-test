#define DEBUG Serial
#define BT Serial4

#define BOUD_RATE 230400
#define WAIT 2000
#define SWITCH 29

#define SAMPLE_RATE 44100
#define FACTOR 8

#define PRE_CMD "\r\n+"
#define POST_CMD "\r\n"

#include <Audio.h>

AudioInputI2S audioInput;
AudioEffectBitcrusher bitCrusher;
AudioRecordQueue queue1;
AudioConnection patchCord1(audioInput, 0, bitCrusher, 0);
AudioConnection patchCord2(bitCrusher, 0, queue1, 0);
AudioControlSGTL5000 audioShield;

void setup()
{
  #ifdef DEBUG
    DEBUG.begin(BOUD_RATE);
    DEBUG.println("SETUP");
  #endif
  
  pinMode(SWITCH, INPUT);
  
  AudioMemory(100);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(40);
  audioShield.volume(0.5);
  
  initBT();

  bitCrusher.sampleRate(SAMPLE_RATE / FACTOR);

  #ifdef DEBUG
    DEBUG.println("LOOP");
  #endif
}

bool record = false;

void loop(){
  bool shouldRecord = digitalRead(SWITCH) == HIGH;
  if(record && !shouldRecord){
    record = false;
    queue1.end();
    #ifdef DEBUG
      DEBUG.println("STOP REC");
    #endif
  }
  else if(!record && shouldRecord){
    record = true;
    queue1.begin();
    #ifdef DEBUG
      DEBUG.println("START REC");
    #endif
  }
  
  if(queue1.available() > 0 && record){
    BT.write((byte*)queue1.readBuffer(), 256);
    queue1.freeBuffer();
  }
  
  #ifdef DEBUG
    static unsigned long last = 0;
    unsigned long mills = millis();
    if(record && mills - last > 1000)
    {
      last = mills;
      DEBUG.println(" ");
      DEBUG.println("#######################");
      DEBUG.println(queue1.available());
      DEBUG.println(DEBUG.availableForWrite());
      DEBUG.println(BT.availableForWrite());
      DEBUG.println("#######################");
      DEBUG.println(" ");
    }
    if(BT.available())
    {
      DEBUG.write(BT.read());
    }
    if(DEBUG.available())
    {
      BT.write(DEBUG.read());
    }
  #endif
}

void initBT()
{
  BT.begin(BOUD_RATE);
  delay(WAIT * 2);
  sendBT("STWMOD=0");
  sendBT("STWMOD=0");
  //sendBT("STNA=teensy");
  sendBT("STAUTO=0");
  sendBT("STOAUT=1");
  //sendBT("STPIN=0000");
  //sendBT(String("STBD=") + BOUD_RATE);
  sendBT("INQ=1");
}

void sendBT(String cmd)
{
  #ifdef DEBUG
    DEBUG.print("BT COMMAND:");
    DEBUG.println(cmd);
  #endif
  cmd = PRE_CMD + cmd + POST_CMD;
  BT.print(cmd);
  delay(WAIT);
  #ifdef DEBUG
    while(BT.available())
    {
      DEBUG.write(BT.read());
    }
  #endif
}
