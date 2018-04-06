#define DEBUG Serial
#define BT Serial4

#define BOUD_RATE 230400
#define WAIT 2000

#define SAMPLE_RATE 8000
#define AUDIO_BLOCK_SAMPLES 128

#define RIGHT 0
#define LEFT 1

#define SWITCH 29

#define PRE_CMD "\r\n+"
#define POST_CMD "\r\n"



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioSettings_F32 audioSettings(SAMPLE_RATE, AUDIO_BLOCK_SAMPLES);

AudioControlSGTL5000_Extended audio;

AudioInputI2S_F32 mic(audioSettings);
AudioOutputI2S_F32 hP(audioSettings);
AudioConvert_F32toI16 converter;
AudioAnalyzeFFT1024 myFFT;

AudioConnection_F32 mic2hPR(mic, RIGHT, hP, RIGHT);
AudioConnection_F32 mic2hPL(mic, LEFT, hP, LEFT);
AudioConnection_F32 mic2conR(mic, RIGHT, converter, RIGHT);
AudioConnection_F32 mic2conL(mic, LEFT, converter, LEFT);
AudioConnection con2recR(converter, RIGHT, myFFT, RIGHT);
AudioConnection con2recL(converter, LEFT, myFFT, LEFT);



void setup()
{
  #ifdef DEBUG
    DEBUG.begin(BOUD_RATE);
    while(!DEBUG);
    DEBUG.println("SETUP");
  #endif

  pinMode(SWITCH, INPUT);

  AudioMemory(100);
  AudioMemory_F32(100, audioSettings);

  audio.enable();
  audio.inputSelect(AUDIO_INPUT_MIC);
  audio.volume(0.2);
  /*audio.lineInLevel(10, 10);
  audio.adcHighPassFilterDisable();
  audio.micBiasEnable(3.0);*/

  initBT();

  #ifdef DEBUG
    DEBUG.println("LOOP");
  #endif
}


void loop()
{
  static bool record = false;
  bool shouldRecord = digitalRead(SWITCH) == HIGH;
  if(record && !shouldRecord){
    record = false;
    #ifdef DEBUG
      DEBUG.println("STOP REC");
    #endif
  }
  else if(!record && shouldRecord){
    record = true;
    #ifdef DEBUG
      DEBUG.println("START REC");
    #endif
  }
  
  if(myFFT.available() && record){
    BT.write((byte*)myFFT.output, 1024);
  }
  
  #ifdef DEBUG
    static unsigned long last = 0;
    unsigned long mills = millis();
    if(record && mills - last > 1000)
    {
      last = mills;
      DEBUG.println(" ");
      DEBUG.println("#######################");
      DEBUG.println(myFFT.available());
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
