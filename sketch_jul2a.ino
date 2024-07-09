#include "DFRobot_SpeechSynthesis_V2.h"
#include <SoftwareSerial.h>

#include <DHT11.h>
#include <DFRobot_ASR.h>
#include <DFRobot_Microwave_Radar_Module.h>
#include <Wire.h>
#include <DFRobot_BMP3XX.h>
/* If you do not need to eliminate the absolute difference of measurement, please comment the following line */
#define CALIBRATE_ABSOLUTE_DIFFERENCE

SoftwareSerial softSerial(/*rx =*/7, /*tx =*/6);
DFRobot_Microwave_Radar_Module Sensor(/*softSerial =*/&softSerial);

// SoftwareSerial ssSerial1(8, 9);  //RX, TX
// DFRobot_SpeechSynthesis_UART ss;
DFRobot_SpeechSynthesis_I2C ss;

DFRobot_ASR asr;

DHT11 dht11(5);
int BH1750address = 0x23;
byte buff[2];
long val3 = 0;
long lightLevel = 0;
long h2 = 0;
long t2 = 0;
long pressure2 = 0;
long altitude2 = 0;
long soundLevel = 0;
long temperature2 = " ";
int h1 = 0;  // 读湿度
int t1 = 0;  // 读温度，默认为摄氏度
char buffer[10];
uint8_t csPin = 4;

DFRobot_BMP388_SPI sensor(&SPI, csPin);
// DFRobot_BMP390L_SPI sensor(&SPI, csPin);




/* Interrupt flag */
volatile uint8_t flag = 0;
/* External interrupt flag */
void interrupt() {
  if (flag == 0) {
    flag = 1;
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  ss.begin();


  //初始化传感器
  while ( !( Sensor.begin() ) ) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  // Serial.println("Begin ok!");


  BH1750_Init(BH1750address);  // 初始化 BH1750 光照传感器
  delay(200);

  int rslt;
  while (ERR_OK != (rslt = sensor.begin())) {
    if (ERR_DATA_BUS == rslt) {
      Serial.println("Data bus error!!!");
    } else if (ERR_IC_VERSION == rslt) {
      Serial.println("Chip versions do not match!!!");
    }
    delay(3000);
  }
  // Serial.println("Begin ok!");
  sensor.setINTMode(sensor.eINTPinPP | sensor.eINTPinActiveLevelHigh | sensor.eINTLatchDIS | sensor.eIntFWtmDis | sensor.eINTFFullDIS | sensor.eINTInitialLevelLOW | sensor.eINTDataDrdyEN);

  delay(100);
#ifdef CALIBRATE_ABSOLUTE_DIFFERENCE
  /**
   * Calibrate the sensor according to the current altitude
   * In this example, we use an altitude of 540 meters in Wenjiang District of Chengdu (China). 
   * Please change to the local altitude when using it.
   * If this interface is not called, the measurement data will not eliminate the absolute difference
   * Note: This interface is only valid for the first call
   */
  if (sensor.calibratedAbsoluteDifference(540.0)) {
    Serial.println("Absolute difference base value set successfully!");
  }
#endif

#if defined(ESP32) || defined(ESP8266)
  // D6 pin is used as interrupt pin by default, other non-conflicting pins can also be selected as external interrupt pins.
  attachInterrupt(digitalPinToInterrupt(D6) /* Query the interrupt number of the D6 pin */, interrupt, CHANGE);
#elif defined(Arduino_SAM_ZERO)
  // Pin 5 is used as interrupt pin by default, other non-conflicting pins can also be selected as external interrupt pins
  attachInterrupt(digitalPinToInterrupt(5) /* Query the interrupt number of the 5 pin */, interrupt, CHANGE);
#else
  /* The Correspondence Table of AVR Series Arduino Interrupt Pins And Terminal Numbers
     * ---------------------------------------------------------------------------------------
     * |                                        |  DigitalPin  | 2  | 3  |                   |
     * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
     * |                                        | Interrupt No | 0  | 1  |                   |
     * |-------------------------------------------------------------------------------------|
     * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
     * |               Mega2560                 |--------------------------------------------|
     * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
     * |-------------------------------------------------------------------------------------|
     * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
     * |    Leonardo, other 32u4-based          |--------------------------------------------|
     * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
     * |--------------------------------------------------------------------------------------
     * ---------------------------------------------------------------------------------------------------------------------------------------------
     *                      The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
     * ---------------------------------------------------------------------------------------------------------------------------------------------
     * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
     * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
     * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
     * |-------------------------------------------------------------------------------------------------------------------------------------------|
     */
  attachInterrupt(/*Interrupt No*/ 1, interrupt, CHANGE);  // Open the external interrupt 0, connect INT1/2 to the digital pin of the main control:
                                                           // UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
#endif

  float sampingPeriodus = sensor.getSamplingPeriodUS();
  // Serial.print("samping period : ");
  // Serial.print(sampingPeriodus);
  // Serial.println(" us");
  // Serial.println("Begin ok1!");
  float sampingFrequencyHz = 1000000 / sampingPeriodus;
  // Serial.print("samping frequency : ");
  // Serial.print(sampingFrequencyHz);
  // Serial.println(" Hz");
  // Serial.println("Begin ok2!");
  Serial.println();
  val3 = 0;
  asr.begin(asr.LOOP);
  // Serial.println("Begin ok6!");
  asr.addCommand("xian zai de wen du si duo shao", 0);
  asr.addCommand("xian zai de qi ya shi duo shao", 2);
  asr.addCommand("xiao chuang ni hao", 3);
  asr.addCommand("xiao chuang zai jian", 4);
  // Serial.println("Begin ok7!");
  //开始识别
  asr.start();
  // Serial.println("Begin ok8!");

  // Serial.println("Begin ok9!");
  ss.begin();
  // Serial.println("Begin ok10!");

  // 设置语音的音量大小为5/
  ss.setVolume(10);
  // Serial.println("Begin ok11!");
  //设置英文以单词发音
  ss.setEnglishPron(ss.eWord);
  // Serial.println("Begin ok12!");
  ss.speak(F("[c5]路灯初始化成功"));
  // Serial.println("Begin ok13!");
    /**
     @brief 恢复出厂设置
  */
  Sensor.factoryReset();

  /**
     @brief 配置检测距离，0~11米，默认为6米
  */
  Sensor.detRangeCfg(7);

  /**
     @brief 配置检测灵敏度，0~9数值越大，灵敏度越高，默认为7
  */
  Sensor.setSensitivity(3);

  /**
     @brief 配置输出延时时间
  */
  Sensor.outputLatency(1, 3);

  /**
     @brief 配置输出输出控制信号接口极性
  */
  Sensor.setGpioMode(1);
  Serial.println("Start");
}

void loop() {
  uint16_t val = 0;
  if (2 == BH1750_Read(BH1750address))  // 读取光照传感器数据
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;  // 计算光照强度值
  }
  lightLevel = val;
  float temperature = sensor.readTempC();
  temperature2 = temperature * 100;
  float Pressure = sensor.readPressPa();
  pressure2 = Pressure * 100;
  readDHTSensor();
  int val888 = Sensor.readPresenceDetection();
  val3=val888;
  // Serial.println(val888);
  switch (asr.read())  //判断识别内容，在有识别结果的情况下asr.read()会返回识别到的词条编号，否则返回-1
  {
    case 0:  //若是指令“da kai deng guang”
      dtostrf(temperature, 6, 2, buffer);

      ss.speak(F("[c5]现在的气温是[n2]"));
      delay(50);
      ss.speak(buffer);
      delay(50);
      ss.speak(F("摄氏度"));
      delay(50);
      itoa(h1, buffer, 10);
      ss.speak(F("[c5]现在的湿度是[n2]百分之"));
      delay(50);
      ss.speak(buffer);
      break;
    case 1:  //若是指令“guan bi deng guang”                                      //熄灭LED

      break;
    case 2:  //若是指令“guan bi deng guang”                                   //熄灭LED
      dtostrf(Pressure / 1000, 8, 2, buffer);
      ss.speak(F("[c5]现在的气压是[n2]"));
      delay(50);
      ss.speak(buffer);
      delay(50);
      ss.speak(F("千帕"));
      break;
    case 3:  //若是指令“guan bi deng guang”                                       //熄灭LED
      ss.speak(F("我叫小创，很高兴为您服务，您可以对我说，现在的温度是多少来获取信息。"));
      break;
    case 4:  //若是指令“guan bi deng guang”                                       //熄灭LED
      ss.speak(F("很高兴为您服务，祝您生活愉快"));
      break;
  }
  sandcgq(0x01, temperature2, h2, lightLevel);
  sandcgq(0x02, val3, pressure2, 0);

  // // delay(2000);
}
void readDHTSensor() {
  h1 = 0;  // 读湿度
  t1 = 0;  // 读温度，默认为摄氏度
  dht11.readTemperatureHumidity(t1, h1);
  if (isnan(h1) || isnan(t1)) {
    //Serial.println("DHT传感器: 读取DHT传感器失败!");
    h2 = -1;
    t2 = -1;
  } else {
    // h2 = String(long(h1*100));
    // t2 = String(long(t1*100));
    h2 = h1 * 100;
    t2 = t1 * 100;
    // Serial.print("DHT传感器湿度: ");
    // Serial.print(h1);
    // Serial.println(" %");
    // Serial.print("DHT传感器温度: ");
    // Serial.print(t1);
    // Serial.println(" ℃");
  }
}

int BH1750_Read(int address) {
  int i = 0;

  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);

  while (Wire.available()) {
    buff[i] = Wire.read();  // 接收传感器数据
    i++;
  }

  Wire.endTransmission();
  return i;  // 返回接收的字节数
}
void readLightSensor() {
  lightLevel = analogRead(A0);
  Serial.print("光强: ");
  Serial.println(lightLevel);
  // delay(1000);
}
void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x10);  // 发送初始化命令，设置传感器工作模式
  Wire.endTransmission();
}
void sandcgq(char address, long a1, long a2, long a3) {
  byte hexArray1[4];
  byte hexArray2[4];
  byte hexArray3[4];

  longToBytes(a1, hexArray1);
  longToBytes(a2, hexArray2);
  longToBytes(a3, hexArray3);

  // 拼接数据到一个数组中
  byte dataToSend[16];  // 增加一个字节，以包含完整的数据
  dataToSend[0] = 0xAA;
  dataToSend[1] = address;
  memcpy(dataToSend + 2, hexArray1, 4);
  memcpy(dataToSend + 6, hexArray2, 4);
  memcpy(dataToSend + 10, hexArray3, 4);
  dataToSend[14] = 0x00;
  dataToSend[15] = 0xBB;

  // 打印拼接好的数据
  // Serial.print("发送数据: ");
  // for (int i = 0; i < sizeof(dataToSend); i++) {
  //   if (dataToSend[i] < 0x10) {
  //     Serial.print("0x0");
  //   } else {
  //     Serial.print("0x");
  //   }
  //   Serial.print(dataToSend[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();

  // 发送拼接好的数据
  Serial.write(dataToSend, sizeof(dataToSend) + 1);
  //  Serial.write(0x47);
  // Serial.println();
  delay(1000);
}

void longToBytes(long number, byte* bytes) {
  bytes[0] = (number >> 24) & 0xFF;  // 最高字节
  bytes[1] = (number >> 16) & 0xFF;
  bytes[2] = (number >> 8) & 0xFF;
  bytes[3] = number & 0xFF;  // 最低字节
}
