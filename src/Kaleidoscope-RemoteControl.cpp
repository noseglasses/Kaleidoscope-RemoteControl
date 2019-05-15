/* -*- mode: c++ -*-
 * Kaleidoscope-RemoteControl -- Controls the firmware via serial communication
 *                               with the host
 * Copyright (C) 2019, noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Kaleidoscope-RemoteControl.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {

namespace remote_control {
   
// void confirmReceive() {
//    ::LEDControl.set_all_leds_to(CRGB(0, 255, 0));
//    ::LEDControl.syncLeds();
//    delay(500);
//    ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
//    ::LEDControl.syncLeds();
// } 
// void confirmSend() {
//    ::LEDControl.set_all_leds_to(CRGB(0, 0, 255));
//    ::LEDControl.syncLeds();
//    delay(500);
//    ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
//    ::LEDControl.syncLeds();
// }

void onFocusReceiveError() {
   ::LEDControl.set_all_leds_to(CRGB(255, 0, 0));
   ::LEDControl.syncLeds();
   delay(500);
   ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
   ::LEDControl.syncLeds();
}

} // namespace remote_control
   
EventHandlerResult RemoteControl::onFocusEvent(const char *command) {

   static constexpr uint8_t cmd_length = 14;
   
   if(strncmp_P(command, PSTR("remote_control"), cmd_length) != 0) {
      return EventHandlerResult::OK;
   }
   
   if(   (command[cmd_length] != '\0') 
      && (strcmp_P(command + cmd_length + 1, PSTR("help")) == 0)) {
      remote_control::help();
      return EventHandlerResult::OK;
   }
   
   uint8_t id = 0;
   Focus.read(id);
  
   remote_control::process(id);
   
   return EventHandlerResult::OK;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::RemoteControl RemoteControl;
