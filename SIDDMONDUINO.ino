/*
  SIDDMONDUINO PROGRAM

  Turns an LED on for one second, then off for one second, repeatedly.

                        ATMEL ATmega32
             
                          +---\\---+
       (XCK/T0) D0 PB0  01|        |40  PA0 AI7 D31 (ADC0)
           (T1) D1 PB1  02|        |39  PA1 AI6 D30 (ADC1)
    (INT2/AIN0) D2 PB2  03|        |38  PA2 AI5 D29 (ADC2)
     (OC0/AIN1) D3 PB3  04|        |37  PA3 AI4 D28 (ADC3)
           (SS) D4 PB4  05|        |36  PA4 AI3 D27 (ADC4)
         (MOSI) D5 PB5  06|        |35  PA5 AI2 D26 (ADC5)
         (MISO) D6 PB6  07|        |34  PA6 AI1 D25 (ADC6)
          (SCK) D7 PB7  08|        |33  PA7 AI0 D24 (ADC7)
                 RESET  09|        |32  AREF
                   VCC  10|        |31  GND
                   GND  11|        |30  AVCC
                 XTAL2  12|        |29  PC7 D23 (TOSC2)
                 XTAL1  13|        |28  PC6 D22 (TOSC1)
          (RXD) D8 PD0  14|        |27  PC5 D21 (TDI)
          (TXD) D9 PD1  15|        |26  PC4 D20 (TDO)
        (INT0) D10 PD2  16|        |25  PC3 D19 (TMS)
        (INT1) D11 PD3  17|        |24  PC2 D18 (TCK)
        (OC1B) D12 PD4  18|        |23  PC1 D17 (SDA)
        (OC1A) D13 PD5  19|        |22  PC0 D16 (SCL)
        (ICP1) D14 PD6  20|        |21  PD7 D15 (OC2)
                          +--------+
  
  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink

  Talkie Library: https://www.arduino.cc/reference/en/libraries/talkie/
  Text-2-Speech Convertor: https://www.instructables.com/Arduino-TTS-Text-to-Speech-/
  Happy Birthday: https://create.arduino.cc/projecthub/Sparkbuzzer/happy-birthday-tone-using-arduino-uno-fba4a8
  Sample Push BTN:  https://create.arduino.cc/projecthub/ronbentley1/button-switch-using-an-external-interrupt-7879df
*/
// Input Pins
#define BTN_INPUT_A1 12
#define BTN_INPUT_A2 13
#define BTN_INPUT_A3 14
#define DIFF_SW_INPUT 16

// Output Pins
#define SPKR_OUTPUT 15
#define LED_BLUE 28
#define LED_GREEN 29
#define LED_RED 30
#define LED_YELLOW 31

// Program Specific Constants
#define debounce_time 50
#define time_delay  100
#define flash_delay 1000
#define tempo 800

// Program storage variables
int btn_input[] = {0, 0, 0};
int task_no = 0, prev_task = 0;
bool game_start = false;
long randNumber;

const char notes[] = "CDEFG";
const int beats[] = {3,3,3,3,5};
const int easy_seq_length = 8;
const int hard_seq_length = 14;

int seq_length = easy_seq_length;
int seq_list[hard_seq_length];
int curr_seq;


// the setup function runs once when you press reset or power the board
void setup() {
  // INPUT FROM 8 to 3 DECODER & THE DIFFICULTY SWITCH
  pinMode(BTN_INPUT_A1, INPUT);
  pinMode(BTN_INPUT_A2, INPUT);
  pinMode(BTN_INPUT_A3, INPUT);
  pinMode(DIFF_SW_INPUT, INPUT);

  // Output to speaker & LEDs
  pinMode(SPKR_OUTPUT, OUTPUT);
  pinMode(LED_BLUE, OUTPUT); pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT); pinMode(LED_YELLOW, OUTPUT);
  Serial.begin(19200);
  randomSeed(analogRead(0));
}

// the loop function runs over and over again forever

//    task_no = readtask();
//    Serial.println("TASK NO.: "+ String(task_no));
//    gameselect(task_no);
void loop() {
  if(game_start == true){
    for (long i = 0; i < seq_length; i += 1){
      Serial.print(i); Serial.println(seq_list[i]);
      gameselect(seq_list[i]);
      gameselect(0);
    }
    game_start = false;
  } else {
    if (readtask() == 1){
      // Based on input from difficulty Switch, generate the
      // sequence of the LED
      if (digitalRead(DIFF_SW_INPUT)){
        seq_length = hard_seq_length;
      }else{
        seq_length = easy_seq_length;
      }
      for (long i = 0; i < seq_length; i += 1){
        randNumber = random(4,8);
        seq_list[i] = randNumber;
      }

      // Game should be in the start mode
      game_start = true;
    }
  }
  delay(time_delay);
}

void gameselect(int task_no){
  if (prev_task == 0){
    switch (task_no){
      case 7:
        strobelight(LED_BLUE,3);
        break;
      case 6:
        strobelight(LED_RED,2);
        break;
      case 5:
        strobelight(LED_GREEN,1);
        break;
      case 4:
        strobelight(LED_YELLOW,0);
        break;
      default:
        break;
    }
    prev_task = task_no;
  } else if (task_no == 0){
    prev_task = 0;
  }
}

int readtask(){
  delay(debounce_time);
  
  int result;
  btn_input[2] = !digitalRead(BTN_INPUT_A1); 
  btn_input[1] = !digitalRead(BTN_INPUT_A2);
  btn_input[0] = !digitalRead(BTN_INPUT_A3); //  Serial.println("INPUT VALUE: "+ String(btn_input[0])+ String(btn_input[1])+ String(btn_input[2]));
  result = btn_input[0]*4 + btn_input[1]*2 + btn_input[2]*1;
  return result;
}

void strobelight(int ledpin, int note){
  digitalWrite(ledpin, HIGH);   // turn the LED on (HIGH is the voltage level)
  playNote(notes[note],beats[note]*tempo);
  digitalWrite(ledpin, LOW);    // turn the LED off by making the voltage LOW 
}

void playNote(char note, int duration) {
  char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                   'c', 'd', 'e', 'f', 'g', 'a', 'b','x', 'y' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                   956,  834,  765,  593,  468,  346,  224, 655 , 715 };
  int SPEE = 5;
  // play the tone corresponding to the note name
  for (int i = 0; i < 17; i++) {
     if (names[i] == note) {
      int newduration = duration/SPEE;
       playTone(tones[i], newduration);
     }
  }
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
     digitalWrite(SPKR_OUTPUT, HIGH);
     delayMicroseconds(tone);
     digitalWrite(SPKR_OUTPUT, LOW);
     delayMicroseconds(tone);
  }
}
