#include <Bounce.h>

const int channel = 1;
const int ON_CC = 63;
const int OFF_CC = 64;
int button_0_state = true;
int button_1_state = true;

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(14, 5);
Bounce button1 = Bounce(15, 5);  // 5 = 5 ms debounce time
//Bounce button2 = Bounce(2, 5);  // which is appropriate for good
//Bounce button3 = Bounce(3, 5);  // quality mechanical pushbuttons
//Bounce button4 = Bounce(4, 5);
//Bounce button5 = Bounce(5, 5);  // if a button is too "sensitive"
//Bounce button6 = Bounce(6, 5);  // to rapid touch, you can
//Bounce button7 = Bounce(7, 5);  // increase this time.

void setup() {
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.  LOW for "on", and HIGH for "off" may seem
  // backwards, but using the on-chip pullup resistors is very
  // convenient.  The scheme is called "active low", and it's
  // very commonly used in electronics... so much that the chip
  // has built-in pullup resistors!
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

}

void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();
  if (button0.fallingEdge()) {
    //    usbMIDI.sendNoteOn(60, 99, channel);  // 60 = C4
  }
  if (button1.fallingEdge()) {
    //    usbMIDI.sendNoteOn(62, 99, channel);  // 60 = C4
  }
  if (button0.risingEdge()) {
    //    usbMIDI.sendNoteOff(60, 0, channel);  // 60 = C4
    if (button_0_state) {
      usbMIDI.sendControlChange(80, ON_CC, channel);
    }
    else {
      usbMIDI.sendControlChange(80, OFF_CC, channel);
    }
    button_0_state = !button_0_state;
  }
  if (button1.risingEdge()) {
    if (button_1_state) {
      usbMIDI.sendControlChange(81, ON_CC - 20, channel);
    }
    else {
      usbMIDI.sendControlChange(81, OFF_CC + 20, channel);
    }
    button_1_state = !button_1_state;
  }
  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
