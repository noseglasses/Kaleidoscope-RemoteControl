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

#pragma once

#include "kaleidoscope/plugin.h"

#define DIRECT_SET(OP, ...)       OP##_DIRECT_SET(__VA_ARGS__)
#define DIRECT_GET(OP, ...)       OP##_DIRECT_GET(__VA_ARGS__)

#define OBJECT_SETTER(OP, ...)    OP##_OBJECT_SETTER(__VA_ARGS__)
#define OBJECT_GETTER(OP, ...)    OP##_OBJECT_GETTER(__VA_ARGS__)

#define GLOBAL_SETTER(OP, ...)    OP##_GLOBAL_SETTER(__VA_ARGS__)
#define GLOBAL_GETTER(OP, ...)    OP##_GLOBAL_GETTER(__VA_ARGS__)

#define OBJECT_TRIGGER(OP, ...)   OP##_OBJECT_TRIGGER(__VA_ARGS__)
#define GLOBAL_TRIGGER(OP, ...)   OP##_GLOBAL_TRIGGER(__VA_ARGS__)
      
//******************************************************************************
// Receive operations
//******************************************************************************
   
#define SWITCH_RECEIVE_DIRECT_SET(ID, DATUM      )                             \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         if(Focus.isEOL()) {                                            __NL__ \
            onFocusReceiveError();                                      __NL__ \
            break;                                                      __NL__ \
         }                                                              __NL__ \
         Focus.read(DATUM);                                             __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_RECEIVE_DIRECT_GET(...) 
      
#define SWITCH_RECEIVE_OBJECT_SETTER(ID, OBJECT, SETTER)                       \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         if(Focus.isEOL()) {                                            __NL__ \
            onFocusReceiveError();                                      __NL__ \
            break;                                                      __NL__ \
         }                                                              __NL__ \
         typedef SetterMethodArgumentTraits<                            __NL__ \
                    decltype(&decltype(OBJECT)::SETTER)                          __NL__ \
                 >::type ArgType;                                       __NL__ \
         ArgType tmp{};                                                 __NL__ \
         Focus.read(tmp);                                               __NL__ \
         (OBJECT).SETTER(tmp);                                          __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_RECEIVE_OBJECT_GETTER(...)
      
#define SWITCH_RECEIVE_GLOBAL_SETTER(ID, SETTER)                               \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         if(Focus.isEOL()) {                                            __NL__ \
            onFocusReceiveError();                                      __NL__ \
            return;                                                     __NL__ \
         }                                                              __NL__ \
         typedef GlobalSetterArgumentTraits<                            __NL__ \
                    decltype(&SETTER)                                   __NL__ \
                 >::type ArgType;                                       __NL__ \
         ArgType tmp{};                                                 __NL__ \
         Focus.read(tmp);                                               __NL__ \
         SETTER(tmp);                                                   __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_RECEIVE_GLOBAL_GETTER(...)
      
#define SWITCH_RECEIVE_OBJECT_TRIGGER(...)
#define SWITCH_RECEIVE_GLOBAL_TRIGGER(...)
      
//******************************************************************************
// Send operations
//******************************************************************************
   
#define SWITCH_SEND_DIRECT_SET(...)
      
#define SWITCH_SEND_DIRECT_GET(ID, DATUM)                                      \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         Focus.send(DATUM);                                             __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_SEND_OBJECT_SETTER(...)
      
#define SWITCH_SEND_OBJECT_GETTER(ID, OBJECT, GETTER)                          \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         Focus.send((OBJECT).GETTER());                                 __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_SEND_GLOBAL_SETTER(...)
      
#define SWITCH_SEND_GLOBAL_GETTER(ID, GETTER)                                  \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         Focus.send(GETTER());                                          __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_SEND_OBJECT_TRIGGER(...)
#define SWITCH_SEND_GLOBAL_TRIGGER(...)
      
//******************************************************************************
// Trigger operations
//******************************************************************************
   
#define SWITCH_TRIGGER(ID)                                                     \
      case ID:                                                          __NL__ \
         IOHelper<ID>::trigger();                                       __NL__ \
      break;
   
#define SWITCH_TRIGGER_DIRECT_SET(...)
#define SWITCH_TRIGGER_DIRECT_GET(...)
      
#define SWITCH_TRIGGER_OBJECT_SETTER(...)
#define SWITCH_TRIGGER_OBJECT_GETTER(...)
      
#define SWITCH_TRIGGER_GLOBAL_SETTER(...)
#define SWITCH_TRIGGER_GLOBAL_GETTER(...)
      
#define SWITCH_TRIGGER_OBJECT_TRIGGER(ID, OBJECT, TRIGGER)                     \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         (OBJECT).TRIGGER();                                            __NL__ \
      }                                                                 __NL__ \
      break;
      
#define SWITCH_TRIGGER_GLOBAL_TRIGGER(ID, TRIGGER)                             \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         TRIGGER();                                                     __NL__ \
      }                                                                 __NL__ \
      break;

//******************************************************************************
      
#define REMOTE_CONTROL_INIT(REMOTE_CONTROL_DEFINITION)                         \
                                                                               \
   namespace kaleidoscope {                                             __NL__ \
   namespace plugin {                                                   __NL__ \
   namespace remote_control {                                           __NL__ \
                                                                        __NL__ \
      void receive(uint8_t entry_id) {                                  __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(SWITCH_RECEIVE)                   __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      void send(uint8_t entry_id) {                                     __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(SWITCH_SEND)                      __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      void trigger(uint8_t entry_id) {                                  __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(SWITCH_TRIGGER)                   __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
   } /* namespace remote_control */                                     __NL__ \
   } /* namespace plugin */                                             __NL__ \
   } /* namespace kaleidoscope */
         
namespace kaleidoscope {
namespace plugin {
namespace remote_control {
   
   extern void receive(uint8_t entry_id);
   extern void send(uint8_t entry_id);
   extern void trigger(uint8_t entry_id);
   
   extern void onFocusReceiveError();
      
   template<typename _T>
   struct StripModifiers {
      typedef _T type;
   };
   
   template<typename _T>
   struct StripModifiers<const _T> {
      typedef _T type;
   };
   
   template<typename _T>
   struct StripModifiers<_T &> {
      typedef _T type;
   };
   
   template<typename _T>
   struct StripModifiers<const _T &> {
      typedef _T type;
   };
   
   template<typename _T>
   struct SetterMethodArgumentTraitsAux {};
   
   template<typename _R, typename _T, typename _A>
   struct SetterMethodArgumentTraitsAux<_R(_T::*)(_A)> {
      typedef _A type;
   };

   template<typename _T>
   struct SetterMethodArgumentTraits {
      typedef typename StripModifiers<typename SetterMethodArgumentTraitsAux<_T>::type>::type type;
   };     
   
   template<typename _T>
   struct GlobalSetterArgumentTraitsAux {};
   
   template<typename _R, typename _A>
   struct GlobalSetterArgumentTraitsAux<_R(*)(_A)> {
      typedef _A type;
   };

   template<typename _T>
   struct GlobalSetterArgumentTraits {
      typedef typename StripModifiers<typename GlobalSetterArgumentTraitsAux<_T>::type>::type type;
   };
} // namespace remote_control

class RemoteControl : public Plugin {
 public:

  EventHandlerResult onFocusEvent(const char *command);
};
} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::RemoteControl RemoteControl;
      
