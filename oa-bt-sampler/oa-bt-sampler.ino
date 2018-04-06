#define DEBUG Serial
#define BT Serial4

#define BOUD_RATE 230400
#define WAIT 1000

#define SAMPLE_RATE 8000
#define AUDIO_BLOCK_SAMPLES 128

#define RIGHT 0
#define LEFT 1

#define SWITCH 29
//#define POT A14

#define PRE_CMD "\r\n+"
#define POST_CMD "\r\n"



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioSettings_F32 audioSettings(SAMPLE_RATE, AUDIO_BLOCK_SAMPLES);

AudioControlSGTL5000_Extended audio;

AudioInputI2S_F32 mic(audioSettings);
AudioOutputI2S_F32 hP(audioSettings);
AudioConvert_F32toI16 converter;
AudioRecordQueue rec;

AudioConnection_F32 mic2hPR(mic, RIGHT, hP, RIGHT);
AudioConnection_F32 mic2hPL(mic, LEFT, hP, LEFT);
AudioConnection_F32 mic2conR(mic/*, RIGHT*/, converter/*, RIGHT*/);
//AudioConnection_F32 mic2conL(mic, LEFT, converter, LEFT);
AudioConnection con2recR(converter/*, RIGHT*/, rec/*, RIGHT*/);
//AudioConnection con2recL(converter, LEFT, rec, LEFT);



int c = 0;
bool record = false;



void setup()
{
  #ifdef DEBUG
    DEBUG.begin(BOUD_RATE);
    while(!DEBUG);
    DEBUG.println("SETUP");
  #endif

  pinMode(SWITCH, INPUT);
  #ifdef POT
    pinMode(POT, INPUT);
  #endif

  initAudio();
  initBT();

  #ifdef DEBUG
    DEBUG.println("LOOP");
  #endif
}



void loop()
{
  changeVolume();
  turnOnOff();
  sendAudio();
  printLog();
}



void changeVolume()
{
  #ifdef POT
    //audio.volume(analogRead(POT) / 1024.0);
    double p = analogRead(POT) / 1024.0 * 15.0;
    audio.lineInLevel(p, p);
  #endif
}



void turnOnOff()
{
  bool shouldRecord = digitalRead(SWITCH) == HIGH;
  if(record && !shouldRecord){
    record = false;
    rec.end();
    #ifdef DEBUG
      DEBUG.println("STOP REC");
      c = 0;
    #endif
  }
  else if(!record && shouldRecord){
    record = true;
    rec.begin();
    #ifdef DEBUG
      DEBUG.println("\nSTART REC");
      c = 0;
    #endif
  }
}



void sendAudio()
{
  if(rec.available()){
    BT.write((byte*)rec.readBuffer(), AUDIO_BLOCK_SAMPLES * 2);
    rec.freeBuffer();
    c++;
  }
}



void printLog()
{
  #ifdef DEBUG
    static unsigned long last = 0;
    unsigned long mills = millis();
    if(record && mills - last > 1000)
    {
      last = mills;
      DEBUG.print("sample rate:  ");
      DEBUG.println(c * 128);
      DEBUG.print("rec:  ");
      DEBUG.println(rec.available());
      c = 0;
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
  delay(WAIT);
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

void initAudio()
{
  AudioMemory(100);
  AudioMemory_F32(100, audioSettings);

  audio.enable();
  audio.inputSelect(AUDIO_INPUT_MIC);
  audio.volume(0.2);
  /*audio.lineInLevel(10, 10);
  audio.adcHighPassFilterDisable();
  audio.micBiasEnable(3.0);*/
}

