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
#include "Kaleidoscope-FocusSerial.h"

#define DIRECT_ACCESS(OP, ...)    OP##_DIRECT_ACCESS(__VA_ARGS__)
#define OBJECT_METHOD(OP, ...)    OP##_OBJECT_METHOD(__VA_ARGS__)
#define GLOBAL_FUNCTION(OP, ...)  OP##_GLOBAL_FUNCTION(__VA_ARGS__)
      
// Process *********************************************************************

#define PROCESS_DIRECT_ACCESS(ID, DATUM)                                       \
      case ID:                                                          __NL__ \
      {                                                                 __NL__ \
         if(::Focus.isEOL()) {                                          __NL__ \
            ::Focus.send(DATUM);                                        __NL__ \
         }                                                              __NL__ \
         else {                                                         __NL__ \
            decltype(DATUM) old_value = DATUM;                          __NL__ \
            ::Focus.read(DATUM);                                        __NL__ \
            ::Focus.send(old_value);                                    __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
      break;
      
#define PROCESS_OBJECT_METHOD(ID, OBJECT, METHOD)                              \
      case ID:                                                          __NL__ \
         memFuncIO(OBJECT, &decltype(OBJECT)::METHOD);                  __NL__ \
         break;
      
#define PROCESS_GLOBAL_FUNCTION(ID, FUNCTION)                                  \
      case ID:                                                          __NL__ \
         globFuncIO(&FUNCTION);                                         __NL__ \
         break;
         
// Help ************************************************************************
         
#define HELP_DIRECT_ACCESS(ID, DATUM)                                          \
         Serial.print(ID);                                               __NL__ \
         printColon();                                                  __NL__ \
         printOSB();                                                    __NL__ \
         printType<decltype(DATUM)>();                                  __NL__ \
         printCSB();                                                    __NL__ \
         printMappedTo();                                               __NL__ \
         printType<decltype(DATUM)>();                                  __NL__ \
         printNewline();
      
#define HELP_OBJECT_METHOD(ID, OBJECT, METHOD)                                 \
         Serial.print(ID);                                               __NL__ \
         printColon();                                                  __NL__ \
         memFuncHelp<decltype(&decltype(OBJECT)::METHOD)>();            __NL__ \
         printNewline();
      
#define HELP_GLOBAL_FUNCTION(ID, FUNCTION)                                     \
         Serial.print(ID);                                               __NL__ \
         printColon();                                                  __NL__ \
         globFuncHelp<decltype(&FUNCTION)>();                           __NL__ \
         printNewline();
  
#define REMOTE_CONTROL_INIT(REMOTE_CONTROL_DEFINITION)                         \
                                                                               \
   namespace kaleidoscope {                                             __NL__ \
   namespace plugin {                                                   __NL__ \
   namespace remote_control {                                           __NL__ \
                                                                        __NL__ \
      void process(uint8_t entry_id) {                                  __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(PROCESS)                          __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      void help() {                                                     __NL__ \
         REMOTE_CONTROL_DEFINITION(HELP)                                __NL__ \
      }                                                                 __NL__ \
   } /* namespace remote_control */                                     __NL__ \
   } /* namespace plugin */                                             __NL__ \
   } /* namespace kaleidoscope */
         
namespace kaleidoscope {
namespace plugin {
namespace remote_control {
   
   extern void process(uint8_t entry_id);
   extern void help();
   
   extern void onFocusReceiveError();
   
   template<typename _T>
   struct TypeNameTrait {
      static const char* name() { return PSTR("<unknown type>"); }
   };
   
   //struct Void {};
   
   #define NAME_TRAIT(TYPE)                                                    \
      template<>                                                        __NL__ \
      struct TypeNameTrait<TYPE> {                                      __NL__ \
         static const char* name() { return PSTR(#TYPE); }              __NL__ \
      };
   NAME_TRAIT(void)
   
   NAME_TRAIT(cRGB)
   NAME_TRAIT(Key)
   NAME_TRAIT(bool)
   NAME_TRAIT(uint8_t)
   NAME_TRAIT(uint16_t)
   NAME_TRAIT(uint32_t)
   #undef NAME_TRAIT
      
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
   typename StripModifiers<_T>::type read() {
      if(::Focus.isEOL()) {
         onFocusReceiveError();
         return typename StripModifiers<_T>::type{};
      }
      typename StripModifiers<_T>::type tmp{};
      ::Focus.read(tmp);
      return tmp;
   }

   template<typename _T> struct MemFuncIO {};

   inline
   void printMappedTo() { 
      Serial.print((const __FlashStringHelper *)PSTR(" -> "));
   }  
   
   inline
   void printSeparator() { 
      Serial.print((const __FlashStringHelper *)PSTR(", "));
   }
   
   inline
   void printOSB() { 
      Serial.print((const __FlashStringHelper *)PSTR("["));
   }
   
   inline
   void printCSB() { 
      Serial.print((const __FlashStringHelper *)PSTR("]"));
   }
   
   inline
   void printColon() { 
      Serial.print((const __FlashStringHelper *)PSTR(": "));
   }
   
   inline
   void printNewline() { 
      Serial.print((const __FlashStringHelper *)PSTR("\n"));
   }
   
   template<typename _T>
   void printType() {
      Serial.print((const __FlashStringHelper *)TypeNameTrait<_T>::name());
   }
   
   template<typename _T>
   uint8_t printSubsequentType() {
      printSeparator();
      Serial.print((const __FlashStringHelper *)TypeNameTrait<_T>::name());
      return 1;
   }
   
   template<typename..._Args>
   void dummy(_Args...) {}

   template<typename _R, typename _T, typename _Arg>
   struct MemFuncIO<_R(_T::*)(_Arg)> {
      
      static void apply(_T &t, _R(_T::*memFunc)(_Arg)) {
         ::Focus.send((t.*memFunc)(read<_Arg>()));
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _R, typename _T, typename _Arg, typename..._Args>
   struct MemFuncIO<_R(_T::*)(_Arg, _Args...)> {
      
      static void apply(_T &t, _R(_T::*memFunc)(_Arg, _Args...)) {
         ::Focus.send((t.*memFunc)(read<_Arg>(), read<_Args>()...));
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _T, typename _Arg>
   struct MemFuncIO<void(_T::*)(_Arg)> {

      static void apply(_T &t, void(_T::*memFunc)(_Arg)) {
         (t.*memFunc)(read<_Arg>());
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _T, typename _Arg, typename..._Args>
   struct MemFuncIO<void(_T::*)(_Arg, _Args...)> {

      static void apply(_T &t, void(_T::*memFunc)(_Arg, _Args...)) {
         (t.*memFunc)(read<_Arg>(), read<_Args>()...);
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _R, typename _T>
   struct MemFuncIO<_R(_T::*)()> {

      static void apply(_T &t, _R(_T::*memFunc)()) {
         ::Focus.send((t.*memFunc)());
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _T>
   struct MemFuncIO<void(_T::*)()> {

      static void apply(_T &t, void(_T::*memFunc)()) {
         (t.*memFunc)();
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<void>();
      }
   }; 

   // Static member functions   
   
   template<typename _R, typename _Arg>
   struct MemFuncIO<_R(*)(_Arg)> {
      
      template<typename _T>
      static void apply(_T &, _R(*memFunc)(_Arg)) {
         ::Focus.send((*memFunc)(read<_Arg>()));
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _R, typename _Arg, typename..._Args>
   struct MemFuncIO<_R(*)(_Arg, _Args...)> {
      
      template<typename _T>
      static void apply(_T &, _R(*memFunc)(_Arg, _Args...)) {
         ::Focus.send((*memFunc)(read<_Arg>(), read<_Args>()...));
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _Arg>
   struct MemFuncIO<void(*)(_Arg)> {

      template<typename _T>
      static void apply(_T &, void(*memFunc)(_Arg)) {
         (*memFunc)(read<_Arg>());
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _Arg, typename..._Args>
   struct MemFuncIO<void(*)(_Arg, _Args...)> {

      template<typename _T>
      static void apply(_T &, void(*memFunc)(_Arg, _Args...)) {
         (*memFunc)(read<_Arg>(), read<_Args>()...);
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _R>
   struct MemFuncIO<_R(*)()> {

      template<typename _T>
      static void apply(_T &, _R(*memFunc)()) {
         ::Focus.send((*memFunc)());
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<>
   struct MemFuncIO<void(*)()> {

      template<typename _T>
      static void apply(_T &, void(*memFunc)()) {
         (*memFunc)();
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<void>();
      }
   };    
   
   template<typename _T, typename _F>
   void memFuncIO(_T &t, _F f) {
      MemFuncIO<decltype(f)>::apply(t, f);
   }
   
   template<typename _F>
   void memFuncHelp() {
      MemFuncIO<_F>::help();
   }

   template<typename _T> struct GlobFuncIO {};
   
   template<typename _R, typename _Arg>
   struct GlobFuncIO<_R(*)(_Arg)> {
      
      static void apply(_R(*memFunc)(_Arg)) {
         ::Focus.send((*memFunc)(read<_Arg>()));
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _R, typename _Arg, typename..._Args>
   struct GlobFuncIO<_R(*)(_Arg, _Args...)> {
      
      static void apply(_R(*memFunc)(_Arg, _Args...)) {
         ::Focus.send((*memFunc)(read<_Arg>(), read<_Args>()...));
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<typename _Arg>
   struct GlobFuncIO<void(*)(_Arg)> {

      static void apply(void(*memFunc)(_Arg)) {
         (*memFunc)(read<_Arg>());
      }
      
      static void help() {
         printType<_Arg>();
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _Arg, typename..._Args>
   struct GlobFuncIO<void(*)(_Arg, _Args...)> {

      static void apply(void(*memFunc)(_Arg, _Args...)) {
         (*memFunc)(read<_Arg>(), read<_Args>()...);
      }
      
      static void help() {
         printType<_Arg>();
         dummy(printSubsequentType<_Args>()...);
         printMappedTo();
         printType<void>();
      }
   };
   
   template<typename _R>
   struct GlobFuncIO<_R(*)()> {

      static void apply(_R(*memFunc)()) {
         ::Focus.send((*memFunc)());
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<_R>();
      }
   };
   
   template<>
   struct GlobFuncIO<void(*)()> {

      static void apply(void(*memFunc)()) {
         (*memFunc)();
      }
      
      static void help() {
         printType<void>();
         printMappedTo();
         printType<void>();
      }
   };       
   
   template<typename _F>
   void globFuncIO(_F f) {
      GlobFuncIO<decltype(f)>::apply(f);
   }   
   
   template<typename _F>
   void globFuncHelp() {
      GlobFuncIO<_F>::help();
   }
} // namespace remote_control

class RemoteControl : public Plugin {
 public:

  EventHandlerResult onFocusEvent(const char *command);
};
} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::RemoteControl RemoteControl;
      
