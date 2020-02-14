/*POWER
 * standby 
 * sleep
 * servo1
 * servo2
 * total for using unlock/ lock
 * improvment with sleep
 * 
 * PSB layout 
 * add power CAP
 * 
 * GPIO 16 is LED
 * 
 * 
 *TODO   //sleep//delay//wakeup//rotating key//make config file. 
 *
 */



#include <ESP32Servo.h>
#include <BluetoothSerial.h>


#define SERVO1_PIN 13 //GPIO13
#define SERVO2_PIN 12 //GPIO12
#define OPEN       0 
#define CLOSE      110
#define swing_out  0
#define swing_in   180
#define TIMER_WIDTH 16
#define SERVO_DELAY 750

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error "Bluetooth is not enabled! Please run `make menuconfig` to and enable it"
#endif

BluetoothSerial SerialBT;
Servo servo1;//13, open, 
Servo servo2;//12, swing,

void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, 500, 2400);
  servo2.attach(SERVO2_PIN, 500, 2400);
  SerialBT.begin("Door_Unlocker");
  Serial.println("start");

 /* while(1){
    lock();
    delay(2000);
    unlock();
    delay(2000);
  }*/
}

void loop() {
  //check if signal
  if (SerialBT.available()) {
    char temp = SerialBT.read();
    if(temp == 'u'){
      Serial.println("unlock");
      digitalWrite(16, HIGH);
      unlock();//unlock door
    }
    if(temp == 'l'){
      Serial.println("lock");
      digitalWrite(16, LOW);
      lock();//lock door
    }
  }
  /*delay(500);
  Serial.println("unlock");
  digitalWrite(16, HIGH);
  unlock();

  delay(500);
  Serial.println("lock");
  digitalWrite(16, LOW);
  lock();*/

}


void unlock(){
  servo1.write(swing_out);
  delay(SERVO_DELAY);
  servo2.write(OPEN);//unlock
  delay(SERVO_DELAY);
  servo2.write(CLOSE);//lock
  delay(SERVO_DELAY);
  servo2.write(OPEN);//unlock
  delay(SERVO_DELAY);
  servo1.write(swing_in);
  delay(SERVO_DELAY);
  SerialBT.write('U');//send singal back
}

void lock(){
  servo1.write(swing_out);//swing out
  delay(SERVO_DELAY);
  servo2.write(CLOSE);//unlock
  delay(SERVO_DELAY);
  servo2.write(OPEN);//lock
  delay(SERVO_DELAY);
  servo2.write(CLOSE);//unlock
  delay(SERVO_DELAY);
  servo1.write(swing_in);//swing back
  delay(SERVO_DELAY);
  SerialBT.write('L');//send singal back 
}
