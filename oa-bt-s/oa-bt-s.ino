#define DEBUG Serial
#define SERIAL Serial4

#define BOUD_RATE 230400
//#define NEW_BOUD_RATE 230400
#define WAIT 2000

#define SAMPLE_RATE 8000
#define AUDIO_BLOCK_SAMPLES 128

#define RIGHT 0
#define LEFT 1

#define SWITCH 29
#define POT A14

#define PRE_CMD "\r\n+"
#define POST_CMD "\r\n"



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioSettings_F32 audioSettings(SAMPLE_RATE, AUDIO_BLOCK_SAMPLES);

AudioControlSGTL5000_Extended audio;

AudioInputI2S_F32 mic(audioSettings);
AudioRecordQueue_F32 rec(audioSettings);
AudioOutputI2S_F32 hP(audioSettings);

AudioConnection_F32 mic2rec(mic, rec);
AudioConnection_F32 mic2hPR(mic, hP);



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
    SERIAL.write((byte*)rec.readBuffer(), AUDIO_BLOCK_SAMPLES * 4);
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
      DEBUG.println(c * AUDIO_BLOCK_SAMPLES);
      DEBUG.print("rec:  ");
      DEBUG.println(rec.available());
      c = 0;
    }
    if(SERIAL.available())
    {
      DEBUG.write(SERIAL.read());
    }
    if(DEBUG.available())
    {
      SERIAL.write(DEBUG.read());
    }
  #endif
}



void initBT()
{
  SERIAL.begin(BOUD_RATE);
  delay(WAIT);
  sendBT("STWMOD=0");
  #ifdef NEW_BOUD_RATE
    sendBT(String("STBD=") + NEW_BOUD_RATE);
  #endif
  sendBT("STWMOD=0");
  //sendBT("STNA=teensy");
  sendBT("STAUTO=0");
  sendBT("STOAUT=1");
  //sendBT("STPIN=0000");
  sendBT("INQ=1");
}



void sendBT(String cmd)
{
  #ifdef DEBUG
    DEBUG.print("SERIAL COMMAND:");
    DEBUG.println(cmd);
  #endif
  cmd = PRE_CMD + cmd + POST_CMD;
  SERIAL.print(cmd);
  delay(WAIT);
  #ifdef DEBUG
    while(SERIAL.available())
    {
      DEBUG.write(SERIAL.read());
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

