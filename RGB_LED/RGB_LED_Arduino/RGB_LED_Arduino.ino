#include <SoftwareSerial.h>

#define TIMEOUT     100  // таймаут в миллисекундах на отработку неправильно посланных данных
#define R_LED_PIN   3
#define G_LED_PIN   5
#define B_LED_PIN   6
#define MOTOR_PIN   2
#define MOTOR_PIN_2 4

SoftwareSerial mySerial(8, 9); // RX, TX
int r_led, g_led, b_led, s_motor, d_motor;
int intValue;
char header;
boolean recievedFlag, startParse;
unsigned long parseTime;

void parsing() {
  if (mySerial.available() > 0) {                 // если есть что то на вход
    char thisChar = mySerial.read();              // принимаем байт
    if (startParse) {                           // если парсим
      if (! isDigit(thisChar)) {                // если приходит НЕ ЦИФРА
        switch (header) {                       // раскидываем по переменным
          case 'r':
            r_led = intValue;
            break;
          case 'g':
            g_led = intValue;
            break;
          case 'b':
            b_led = intValue;
            break;
          case 's':
            s_motor = intValue;
            break;
          case 'd':
            d_motor = intValue;
            break;
        }
        recievedFlag = true;                  // данные приняты
        startParse = false;                   // парсинг завершён
      } else {                                // если принятый байт всё таки цифра
        intValue = intValue * 10 + (thisChar - '0');  // с каждым принятым число растёт слева направо
      }
    }
    if (isAlpha(thisChar) && !startParse) {     // если префикс БУКВА и парсинг не идёт
      header = thisChar;                        // запоминаем префикс
      intValue = 0;                             // обнуляем
      startParse = true;                        // флаг на парсинг
      parseTime = millis();                     // запоминаем таймер
    }
  }
  if (startParse && (millis() - parseTime > TIMEOUT)) {
    startParse = false;                   // парсинг завершён по причине таймаута
  }
};

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
}

void loop() {
  parsing();
  if (recievedFlag) {
    Serial.print(String(r_led) + " " + String(g_led) + " " + String(b_led));
    // выводим в порт для отладки
    analogWrite(R_LED_PIN, r_led);
    analogWrite(G_LED_PIN, g_led);
    analogWrite(B_LED_PIN, b_led);

    if (s_motor == 1) {
      if (d_motor == 1) {
        digitalWrite(MOTOR_PIN, 1);
        digitalWrite(MOTOR_PIN_2, 0);
      }
      else {
        digitalWrite(MOTOR_PIN, 0);
        digitalWrite(MOTOR_PIN_2, 1);
      }
    }
    else {
      analogWrite(MOTOR_PIN, 0);
      analogWrite(MOTOR_PIN_2, 0);
    }
    recievedFlag = false;
  }
}
