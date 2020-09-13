#include <Bounce.h>

#define ARRAYSIZE(x)  (sizeof(x) / sizeof(x[0]))
//needed to keep first interrupts at bay
bool first_run = true;


int channel = 1;

const int pot_pins[] = {14, 15, 16};
int pots[] = {0, 0, 0};
int pot_previous_read[] = {0, 0, 0};
const int pots_cc_number[] = {74, 75, 76};

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
int button_pins[] = {2, 3, 4};
Bounce buttons[] = {
    Bounce(button_pins[0], 5),
    Bounce(button_pins[1], 5),
    Bounce(button_pins[2], 5)
  };

const int button_cc_number[] = {77, 78, 79};
const int toggle_channel_pin = 6;
const int toggle_channel_cc = 80;
Bounce toggle_channel_button = Bounce(toggle_channel_pin, 5);



void setup() {

  pinMode(toggle_channel_pin , INPUT_PULLUP);
  for (int i = 0; i < ARRAYSIZE(button_pins); i++) {
//    buttons[i] = Bounce(button_pins[i], 5);
    pinMode(button_pins[i] , INPUT_PULLUP);
  }

  //  pinMode(14, INPUT_PULLUP);
  //  pinMode(15, INPUT_PULLUP);

  //Read init state for pots and store as previous read
  for (int i = 0; i < ARRAYSIZE(pot_pins); i++) {
    pots[i] = pot_bounce(analogRead(pot_pins[i]));
    pot_previous_read[i] = pots[i];
  }

  Serial.begin(9600);
  //
  //  pot1 = pot_bounce(analogRead(pot_pin1));
  //  previous_read =  pot1;

}
int pot_bounce(int value) {
  return value / 8;
}
void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
//  for (int i = 0; i < ARRAYSIZE(pot_pins); i++) {
//    pots[i] = pot_bounce(analogRead(pot_pins[i]));
//    if (pot_previous_read[i] != pots[i]) {
//      pot_previous_read[i] = pots[i];
//      usbMIDI.sendControlChange(pots_cc_number[i] , pots[i] , channel);
//      Serial.println("sending pot message :");
//      Serial.println(pots_cc_number[i]);
//      Serial.println("sending pot value :");
//      Serial.println(pots[i]);
//    }
//  }
  for(int i = 0; i< ARRAYSIZE(button_pins);i++){
    buttons[i].update();
    }
  toggle_channel_button.update();
  if (first_run){
    
    first_run = false;
    return;
    } 
  for (int i = 0; i < ARRAYSIZE(button_pins); i++) {
    if (buttons[i].risingEdge()) {
        usbMIDI.sendControlChange(button_cc_number[i], 0, channel);
        Serial.println("Button pressed :");
        Serial.println(i);
        Serial.println("cc sent");
        Serial.println(toggle_channel_cc);
      }
  }

  if (toggle_channel_button.risingEdge()) {
        channel = (channel % 16) + 1;
      Serial.println("new channel: ");
      Serial.println(channel);
    }
  

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
