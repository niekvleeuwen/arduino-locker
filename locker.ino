#include <Keypad.h>

String code[4] = {"1", "1", "1", "1"};
String input[4];
int pointer = 0;
int solenoidPin = 9;    //This is the output pin on the Arduino we are using
boolean doorOpen = false;

int piezoPin = 10;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  pinMode(solenoidPin, OUTPUT);           //Sets the pin as an output
  digitalWrite(solenoidPin, HIGH);    //Switch Solenoid ON
}

void loop() {
    char inputKeypad = keypad.getKey(); //read the input from the keypad

    if (inputKeypad != NO_KEY) { //check if the input has a actual value
      String key = (String) inputKeypad; //cast the input to a string
      //Serial.println(key);
      tone(piezoPin, 440, 100);
      delay(100);
      if(doorOpen == true){
         if(key == "#"){
            digitalWrite(solenoidPin, HIGH);     //Switch Solenoid OFF
            Serial.println("Door is locked"); 
            doorOpen = false;
            delay(200);
            tone(piezoPin, 440, 100);
            delay(200);
            tone(piezoPin, 440, 100);
            delay(200);
         }
      }else{
          if(key == "#"){
          boolean correct = true;
          for(int i = 0; i < 4; i++){
            Serial.print("input: ");
            Serial.println(input[i]);
            Serial.print("code: ");
            Serial.println(code[i]);
            if(input[i] != code[i]){
                correct = false;
            }
          }
          if(correct == true){
            digitalWrite(solenoidPin, LOW);    //Switch Solenoid ON
            Serial.println("Door is open");
            doorOpen = true;
          }else{
            Serial.println("Acces denied");
            tone(piezoPin, 440, 100);
            delay(100);
            tone(piezoPin, 440, 100);
            delay(100);
            tone(piezoPin, 440, 100);
            delay(100);
          }
          //clear the input
          for(int i = 0; i < 4; i++){
            input[i] = "";
          }
          pointer = 0;
      }else if(key == "*"){
          if(pointer > 0){
            input[pointer - 1] = "";
            pointer--; 
          }
      }else{
          if(pointer < 4){
              input[pointer] = key;
              pointer++;
          }
      }
      for(int i = 0; i < 4; i++){
         if(input[i] != ""){
             Serial.print(input[i]);          
         }
      }
      Serial.println();
   } 
   }
}