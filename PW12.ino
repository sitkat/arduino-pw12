#define lathchPin 4 //линия синхронизации данных, защелка (LCHCLK)
#define clockPin 7 //линия тактирования (SFTCLK)
#define dataPin 8 //линия передачи данных(SDI)

int number = 0;
String str;


int x = 0;

unsigned long timing_1 = 0;
#define INTERVAL_1 1

int segments[4];
int place = 0;

const byte SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};
const byte SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

void setup() {
  Serial.begin(9600);

  pinMode(lathchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  if (millis() > timing_1 + INTERVAL_1) {
    timing_1 = millis();
    if (place >= 3) place = 0;
    else place++;
    WriteNumberToSegment(place, segments[place]);
  }
  if (Serial.available () > 0) {
    str = Serial.readString();
    if (atoui(str.c_str(), &number)) {
      if (number >= 0 && number <= 9999) {
        Serial.println("Введено число: " + str);
        place = 0;
        segments[0] = segments[1] = segments[2] = segments[3] = place;
        while (number > 0) {
          x = x * 10 + (number % 10);
          number = number / 10;
        }
        while (x > 0) {
          int num = x % 10;
          WriteNumberToSegment(place, (byte)num);
          segments[place] = (byte)num;
          place++;
          x = x / 10;
        }
      }
      else
        Serial.println("Введите максимум 4 цифры");
    }
    else Serial.println("Ошибка ввода");
  }
}


void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(lathchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(dataPin, clockPin, MSBFIRST, SEGMENT_SELECT[Segment]);
  digitalWrite(lathchPin, HIGH);
}
