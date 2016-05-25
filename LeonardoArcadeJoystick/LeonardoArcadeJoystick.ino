/*
  LeonardoArcadeJoystick.cpp
  
  Copyright (c) 2016, Albert Horta (albert@horta.name)
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Joystick.h>

class BasicButton {
  protected:
  uint8_t _pin_number, 
          _previous_state,
          _current_state;

  public:
  BasicButton(uint8_t pin) {
    _pin_number = pin;

    pinMode(_pin_number, INPUT_PULLUP);
    _previous_state = rawState();
  }

  uint8_t getPreviousState() {
    return _previous_state;
  }

  void setPreviousState(uint8_t state) {
    _previous_state = state;
  }

  uint8_t getCurrentState() {
    return _previous_state;
  }

  uint8_t rawState() {
    return !digitalRead(_pin_number);
  }

  void setCurrentState() {
    _current_state = rawState();
  }

  void takeAction() {}

  void activate() {}

  void deactivate() {}
};

class InputButton : public BasicButton {
  protected:
  uint8_t _button_number;

  public:
  InputButton(uint8_t pin, uint8_t button) : BasicButton(pin) {
    _button_number = button;
  }

  void takeAction() {
    uint8_t input_state;
    
    if((input_state = rawState()) != getPreviousState()) {
      Joystick.setButton(_button_number, input_state);
      setPreviousState(input_state);
    }
  }
};

class XPadButton : public BasicButton {
  protected:
  int8_t _pad_value;
  
  public:
  XPadButton(uint8_t pin, int8_t value) : BasicButton(pin) {
    _pad_value = value;
  }

  void takeAction() {
    uint8_t input_state;

    if((input_state = rawState()) != getPreviousState()) {
      if(input_state == 0) {
        Joystick.setXAxis(0);        
      } else {
        Joystick.setXAxis(_pad_value);
      }
      setPreviousState(input_state);
    }    
  }
};

class YPadButton : public BasicButton {
  protected:
  int8_t _pad_value;
  
  public:
  YPadButton(uint8_t pin, int8_t value) : BasicButton(pin) {
    _pad_value = value;
  }

  void takeAction() {
    uint8_t input_state;

    if((input_state = rawState()) != getPreviousState()) {
      if(input_state == 0) {
        Joystick.setYAxis(0);        
      } else {
        Joystick.setYAxis(_pad_value);
      }
      setPreviousState(input_state);
    }    
  }
};

class ClassicLedPWM {
  protected:
  uint8_t _pin_number,
          _current_state;

  public:
  ClassicLedPWM(uint8_t pin) {
    _pin_number = pin;

    pinMode(_pin_number, OUTPUT);
    setLed(0);
  }

  void setLed(uint8_t state) {
    _current_state = state;
    analogWrite(_pin_number, state);
  }

  uint8_t getLed() {
    return _current_state;
  }
};

class TriggeredButton : public InputButton {
  protected:
  uint8_t _triggered_active,
          _triggered_loop_delay,
          _triggered_current_loop,
          _triggered_state;
  ClassicLedPWM* _led;

  public:
  TriggeredButton(uint8_t pin, uint8_t button, ClassicLedPWM* led) : InputButton(pin, button) {
    _triggered_active = 0;
    _triggered_loop_delay = 50;
    _triggered_current_loop = 0;
    _led = led;
  }

  void activate() {
    _triggered_active = 1;
    _triggered_current_loop = 0;
  }

  void deactivate() {
    _triggered_active = 0;
    _triggered_current_loop = 0;
  }

  uint8_t isActive() {
    return _triggered_active;
  }

  uint8_t getLoopDelay() {
    return _triggered_loop_delay;
  }

  void setLoopDelay(uint8_t loop_delay) {
    _triggered_loop_delay = loop_delay;
  }

  uint8_t nextCurrentLoop() {
    _triggered_current_loop = (_triggered_current_loop + 1) % _triggered_loop_delay;
    return _triggered_current_loop;
  }

  void resetCurrentLoop() {
    _triggered_current_loop = 0;
  }

  void takeAction() {
    uint8_t input_state;
    
    if((input_state = rawState()) != getPreviousState()) {
      Joystick.setButton(_button_number, input_state);
      setPreviousState(input_state);
      if(input_state == 1) {
        _triggered_state = 1;
        resetCurrentLoop();
      }
    }

    if(input_state == 1) {
      if(isActive()) {
        if(nextCurrentLoop() == 0) {
          _triggered_state = (_triggered_state + 1) % 2;
          Joystick.setButton(_button_number, _triggered_state);
        }
      }
    }
  }
};

TriggeredButton* btns[12];
XPadButton* xPad[2];
YPadButton* yPad[2];
BasicButton* setTrigger;
BasicButton* resetTrigger;
ClassicLedPWM* led[1];

void setup() {
  Joystick.begin();
  
  // Leds
  led[0] = new ClassicLedPWM(13);
  
  // Control buttons
  setTrigger = new BasicButton(A4);       // Set trigger button mapped to pin A4
  resetTrigger = new BasicButton(A5);     // Reset trigger button mapped to pin A5

  // Pad
  xPad[0] = new XPadButton(A0, -127);     // Left direction mapped to pin A0
  xPad[1] = new XPadButton(A1, +127);     // Right direction mapped to pin A1
  yPad[0] = new YPadButton(A2, -127);     // Up direction md to pin A2appe
  yPad[1] = new YPadButton(A3, +127);     // Down direction mapped to pin A3
  
  // Arcade buttons
  btns[0] = new TriggeredButton(1, 0, led[0]);    // button 0 mapped to pin 0
  btns[1] = new TriggeredButton(2, 1, led[0]);    // button 1 mapped to pin 1
  btns[2] = new TriggeredButton(3, 2, led[0]);    // button 2 mapped to pin 2
  btns[3] = new TriggeredButton(4, 3, led[0]);    // button 3 mapped to pin 3
  btns[4] = new TriggeredButton(5, 4, led[0]);    // button 4 mapped to pin 4
  btns[5] = new TriggeredButton(6, 5, led[0]);    // button 5 mapped to pin 7
  btns[6] = new TriggeredButton(7, 6, led[0]);   // button 6 mapped to pin 8
  btns[7] = new TriggeredButton(8, 7, led[0]);   // button 7 mapped to pin 9
  btns[8] = new TriggeredButton(9, 8, led[0]);  // button 8 mapped to pin 10
  btns[9] = new TriggeredButton(10, 9, led[0]);  // button 9 mapped to pin 11
  btns[10] = new TriggeredButton(11, 10, led[0]); // button 10 mapped to pin 12
  btns[11] = new TriggeredButton(12, 11, led[0]); // button 11 mapped to pin 13
}

void loop() {
  if(setTrigger->rawState() == 1) {
    for(uint8_t i=0; i<12; i++) {
      if(btns[i]->rawState() == 1) {
        btns[i]->activate();
      }
    }
  } else if(resetTrigger->rawState() == 1) {
    for(uint8_t i=0; i<12; i++) {
      if(btns[i]->rawState() == 1) {
        btns[i]->deactivate();
      }
    }
  } else {
    for(uint8_t i=0; i<2; i++) {
      xPad[i]->takeAction();
    }
    for(uint8_t i=0; i<2; i++) {
      yPad[i]->takeAction();
    }
    for(uint8_t i=0; i<12; i++) {
      btns[i]->takeAction();
    }
  }
  
  delay(50);
}
