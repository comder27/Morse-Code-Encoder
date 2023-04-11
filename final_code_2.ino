#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int blueled = 7;
const int yellowled = 3;
const int buttono = 4;  //dot
const int buttona = 5;  //dash
const int buzzer = 6;
const int enter = 2;
int buttonstate = 0;
int enterstate = 0;
char m[8];

char space[5] = { '-', '.', '.', '.', '-' };
String morsecode[] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "/", ".-.-.-", "--..--", "..--..", ".----.", "-.-.--", "-..-.", "-....-", "..--.-", "-.--.", "-.--.-", ".-..-.", ".--.-.", "-...-", ".-.-.", "---...", ".......", "--..--", "-.-.-.", "..--.-" };
String alpha[52] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "/", ".", ",", "?", "!", "-", "'", "(", ")", "\"", "@", "=", "+", "_", "$", "&" };


int cursor = 0;

void blowbuzzer() {
  digitalWrite(buzzer, HIGH);
}
void shutbuzzer() {
  digitalWrite(buzzer, LOW);
}

void glowBLED(int buttonstate) {
  if (buttonstate == HIGH) {
    digitalWrite(blueled, HIGH);
    blowbuzzer();

  } else {
    digitalWrite(blueled, LOW);
    shutbuzzer();
  }
}
void glowYLED(int buttonstate) {
  if (buttonstate == HIGH) {
    digitalWrite(yellowled, HIGH);
    blowbuzzer();
  } else {
    digitalWrite(yellowled, LOW);
    shutbuzzer();
  }
}


void setup() {
  // put your setup code here, to run once:
  //  Initialize the LCD
  lcd.init();

  // Turn on the backlight
  lcd.backlight();

  // Set the cursor to the first column of the first row
  lcd.setCursor(3, 0);
  Serial.begin(9600);
  pinMode(blueled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  pinMode(buttono, INPUT);
  pinMode(buttona, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(enter, INPUT);
  lcd.print("MORSE CODE");
  lcd.setCursor(4, 1);
  lcd.print("DECODER!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
}


void loop() {
  int k = 7;
  int i = 0;
  while (k--) {
    while (1) {
      // for enter
      enterstate = digitalRead(enter);
      if (enterstate == HIGH) {
        break;
      }
      // for dot
      buttonstate = digitalRead(buttono);
      if (buttonstate == HIGH) {
        glowBLED(buttonstate);
        m[i] = '.';
        i++;
        delay(300);
        break;
      } else {
        glowBLED(buttonstate);
      }

      // for dash
      buttonstate = digitalRead(buttona);
      if (buttonstate == HIGH) {
        glowYLED(buttonstate);
        m[i] = '-';
        i++;
        delay(700);
        break;
      } else {
        glowYLED(buttonstate);
      }
    }
    if (enterstate == HIGH) {
      break;
    }
  }

  // print character corresponding to input morse code
  String ch = String(m);
  for (int j = 0; j < 51; j++) {
    int count = 0;
    if (i == morsecode[j].length()) {
      for (int x = 0; x < i; x++) {
        if (m[x] == morsecode[j].c_str()[x]) {
          Serial.print(m[x]);
          count++;
        }
      }
    }

    if (i == count) {
      lcd.print(alpha[j]);
      cursor++;
      break;
    }
  }

  // print space
  int cnt = 0;
  for (int i = 0; i < 5; i++) {
    if (m[i] == space[i]) {
      cnt++;
    }
  }
  if (cnt == 5) {
    lcd.print(" ");
    Serial.print(" ");
    cursor++;
  }
  if (cursor == 16) {
    lcd.setCursor(0, 1);
  }

  // delay for debouncing
  delay(500);
}
