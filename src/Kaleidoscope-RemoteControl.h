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

#include "kaleidoscope_internal/type_traits/has_member.h"
#include "kaleidoscope_internal/type_traits/type_traits"

namespace kaleidoscope {
namespace plugin {
namespace remote_control {
   
struct Dummy {};

template<typename _T1, typename _T2>
struct IsSame { static constexpr bool value = false; };

template<typename _T>
struct IsSame<_T, _T> { static constexpr bool value = true; };
   
template<typename _Dummy, typename _T>
_T passOn(_T t) { return t; }

struct __Undefined__ {};

namespace signature_check {
   template<typename _T, int _entry_id, int _entry_type>
   struct IOHelper {};

   template<int _entry_id>
   struct EntryTypeTraits {};
} // namespace signature_check

#define __EVAL(S) S

#define CAT2(A, B) A##B
#define CAT3(A, B, C) A##B##C

#define INSTANCE_EXISTS_TRAITS(TRAITS_NAME, INSTANCE)                          \
   template<typename _Dummy>                                            __NL__ \
   struct TRAITS_NAME                                                   __NL__ \
   {                                                                    __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr                                                  __NL__ \
      decltype(                                                         __NL__ \
         &INSTANCE, bool{}                                              __NL__ \
      )                                                                 __NL__ \
      test(int /* unused */)                                            __NL__ \
      {                                                                 __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr bool test(...)                                   __NL__ \
      {                                                                 __NL__ \
         return false;                                                  __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr bool value = test<int>(int{});                   __NL__ \
   };

#define MEMBER_CALL_TRAITS(TRAITS_NAME, FUNC)                                   \
   template<typename _ObjType, typename _ArgType>                       __NL__ \
   struct TRAITS_NAME                                                   __NL__ \
   {                                                                    __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr                                                  __NL__ \
      decltype(                                                         __NL__ \
         std::declval(_ObjType).FUNC(std::declval(_ArgType)), bool{}                              __NL__ \
      )                                                                 __NL__ \
      test(int /* unused */)                                            __NL__ \
      {                                                                 __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr bool test(...)                                   __NL__ \
      {                                                                 __NL__ \
         return false;                                                  __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr bool value = test<int>(int{});                   __NL__ \
   };
   
#define GLOBAL_CALL_TRAITS(TRAITS_NAME, FUNC)                                   \
   template<typename _ArgType>                       __NL__ \
   struct TRAITS_NAME                                                   __NL__ \
   {                                                                    __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr                                                  __NL__ \
      decltype(                                                         __NL__ \
         FUNC(std::declval(_ArgType)), bool{}                              __NL__ \
      )                                                                 __NL__ \
      test(int /* unused */)                                            __NL__ \
      {                                                                 __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      static constexpr bool test(...)                                   __NL__ \
      {                                                                 __NL__ \
         return false;                                                  __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr bool value = test<int>(int{});                   __NL__ \
   };

enum EntryType {
   EntryTypeError,
   
   EntryTypeObjSetterGetter,
   EntryTypeGlobalFunctions,
   EntryTypeGlobalIntrinsic
};

} // namespace remote_control
} // namespace plugin
} // namespace kaleidoscope

#define PREPARE_CHECK_SIGNATURES(OBJ, GETTER, SETTER, TYPE)                    \
   PREPARE_CHECK_SIGNATURES_AUX(__COUNTER__, OBJ, GETTER, SETTER, TYPE)

#define PREPARE_CHECK_SIGNATURES_AUX(COUNTER, OBJ, GETTER, SETTER, TYPE)       \
                                                                        __NL__ \
   namespace signature_check {                                          __NL__ \
                                                                        __NL__ \
      INSTANCE_EXISTS_TRAITS(CAT2(TargetIsGlobalInstance_,                      \
                                  COUNTER), OBJ)                        __NL__ \
                                                                        __NL__ \
      MEMBER_CALL_TRAITS(CAT2(InstanceSetterGetterCompatible_,                  \
                             COUNTER), SETTER)      __NL__ \
                                                                        __NL__ \
      GLOBAL_CALL_TRAITS(CAT2(GlobalSetterGetterCompatible_,                    \
                             COUNTER), SETTER)          __NL__ \
                                                                        __NL__ \
      DEFINE_HAS_MEMBER_TRAITS(CAT2(SetterIsInstanceMethod_,                   \
                                    COUNTER),                                  \
                               SETTER)                                  __NL__ \
                                                                        __NL__ \
      DEFINE_HAS_MEMBER_TRAITS(CAT2(GetterIsInstanceMethod_,                   \
                                    COUNTER),                                  \
                               GETTER)                                  __NL__ \
                                                                        __NL__ \
      constexpr int entry_id = COUNTER - first_entry_counter - 1;       __NL__ \
                                                                        __NL__ \
      template<bool /* true */>                                         __NL__ \
      struct BranchAccessorsCompatible {                                __NL__ \
         static constexpr int value = EntryTypeObjSetterGetter;         __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct BranchAccessorsCompatible<false> {                         __NL__ \
         static constexpr int value = EntryTypeError;                   __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<bool /* true */>                                         __NL__ \
      struct BranchAccessorsAvailable {                                 __NL__ \
         static constexpr int value = BranchAccessorsCompatible<        __NL__ \
            CAT2(InstanceSetterGetterCompatible_, COUNTER)<decltype(OBJ), TYPE>::value __NL__ \
         >::value;                                                      __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct BranchAccessorsAvailable<false> {                          __NL__ \
         static constexpr int value = EntryTypeGlobalIntrinsic;         __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T, bool /* true */>                                         __NL__ \
      struct BranchClassInstance {                                      __NL__ \
         static_assert(sizeof(_T) == 0, "NO!"); \
                                                                        __NL__ \
         static constexpr int value = BranchAccessorsAvailable<         __NL__ \
               CAT3(SetterIsInstanceMethod_, COUNTER, _HasMember_##SETTER) __NL__ \
                               <decltype(OBJ)>::value                   __NL__ \
            && CAT3(GetterIsInstanceMethod_, COUNTER, _HasMember_##GETTER) __NL__ \
                               <decltype(OBJ)>::value                   __NL__ \
         >::value;                                                      __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T>                                                        __NL__ \
      struct BranchClassInstance<_T, false> {                               __NL__ \
         static constexpr int value = EntryTypeGlobalIntrinsic;         __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<bool /* true */>                                         __NL__ \
      struct BranchGlobalInstance {                                     __NL__ \
         static_assert(!std::is_class<decltype(OBJ)>::value, "blu"); \
         static constexpr int value = BranchClassInstance<              __NL__ \
            int, std::is_class<decltype(OBJ)>::value                         __NL__ \
         >::value;                                                      __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<bool /* true */>                                         __NL__ \
      struct BranchGlobalGettersAndSettersCompatible {                  __NL__ \
         static constexpr int value = EntryTypeGlobalFunctions;         __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct BranchGlobalGettersAndSettersCompatible<false> {           __NL__ \
         static constexpr int value = EntryTypeError;                   __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct BranchGlobalInstance<false> {                              __NL__ \
         static constexpr int value = BranchGlobalGettersAndSettersCompatible< __NL__ \
            CAT2(GlobalSetterGetterCompatible_, COUNTER)<TYPE>::value  __NL__ \
         >::value;                                                      __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct EntryTypeTraits<entry_id> {                                __NL__ \
         static constexpr int value = BranchGlobalInstance<             __NL__ \
            CAT2(TargetIsGlobalInstance_, COUNTER)<Dummy>::value        __NL__ \
         >::value;                                                      __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      struct IOHelper<_T, entry_id, EntryTypeObjSetterGetter>           __NL__ \
      {                                                                 __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {                                           __NL__ \
            TYPE tmp{};                                                 __NL__ \
            Focus.read(tmp);                                            __NL__ \
            OBJ.SETTER(tmp);                                            __NL__ \
         }                                                              __NL__ \
                                                                        __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void send() {                                           __NL__ \
            Focus.send(OBJ.GETTER());                                   __NL__ \
         }                                                              __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct IOHelper<void, entry_id, EntryTypeObjSetterGetter>         __NL__ \
      {                                                                 __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {                                           __NL__ \
            OBJ.SETTER();                                               __NL__ \
         }                                                              __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void send() {                                           __NL__ \
            OBJ.GETTER();                                               __NL__ \
         }                                                              __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      struct IOHelper<_T, entry_id, EntryTypeGlobalFunctions>           __NL__ \
      {                                                                 __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {                                           __NL__ \
            TYPE tmp{};                                                 __NL__ \
            Focus.read(tmp);                                            __NL__ \
            SETTER(tmp);                                                __NL__ \
         }                                                              __NL__ \
                                                                        __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void send() {                                           __NL__ \
            Focus.send(GETTER());                                       __NL__ \
         }                                                              __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<>                                                        __NL__ \
      struct IOHelper<void, entry_id, EntryTypeGlobalFunctions>         __NL__ \
      {                                                                 __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {                                           __NL__ \
            SETTER();                                                   __NL__ \
         }                                                              __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void send() {                                           __NL__ \
            GETTER();                                                   __NL__ \
         }                                                              __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      struct IOHelper<_T, entry_id, EntryTypeGlobalIntrinsic>           __NL__ \
      {                                                                 __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {                                           __NL__ \
            Focus.read(OBJ);                                            __NL__ \
         }                                                              __NL__ \
                                                                        __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void get() {                                            __NL__ \
            Focus.send(OBJ);                                            __NL__ \
         }                                                              __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      template<typename _T>                                             __NL__ \
      struct IOHelper<_T, entry_id, EntryTypeError>                     __NL__ \
      {                                                                 __NL__ \
         static_assert(false, "\nError in definition of remote "        __NL__ \
                                "control entry\n"                       __NL__ \
                              "   OBJ: " #OBJ "\n"                      __NL__ \
                              "   GETTER: " #GETTER "\n"                __NL__ \
                              "   SETTER: " #SETTER "\n"                __NL__ \
                              "   TYPE: " #TYPE "\n"                    __NL__ \
                              "Unable to setup I/O");                   __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void read() {}                                          __NL__ \
         template<typename _Dummy>                                      __NL__ \
         static void get() {}                                           __NL__ \
      };                                                                __NL__ \
   } /* namespace signature_check */
   
#define SWITCH_READ(OBJ, GETTER, SETTER, TYPE) \
   SWITCH_READ_AUX(__COUNTER__, OBJ, GETTER, SETTER, TYPE)

#define SWITCH_READ_AUX(COUNTER, OBJ, GETTER, SETTER, TYPE)                    \
   case COUNTER - first_entry_counter_read - 1:                         __NL__ \
      IOHelper<TYPE,                                                    __NL__ \
               COUNTER - first_entry_counter_read - 1,                  __NL__ \
               signature_check::EntryTypeTraits<entry_id>::value        __NL__ \
      >::read<Dummy>();                                                 __NL__ \
      break;
      
#define SWITCH_SEND(OBJ, GETTER, SETTER, TYPE) \
   SWITCH_SEND_AUX(__COUNTER__, OBJ, GETTER, SETTER, TYPE)

#define SWITCH_SEND_AUX(COUNTER, OBJ, GETTER, SETTER, TYPE)                    \
   case COUNTER - first_entry_counter_read - 1:                         __NL__ \
      IOHelper<TYPE,                                                    __NL__ \
               COUNTER - first_entry_counter_read - 1,                  __NL__ \
               signature_check::EntryTypeTraits<entry_id>::value        __NL__ \
      >::send<Dummy>();                                                 __NL__ \
      break;

#define REMOTE_CONTROL_INIT(REMOTE_CONTROL_DEFINITION)                         \
                                                                               \
   namespace kaleidoscope {                                             __NL__ \
   namespace plugin {                                                   __NL__ \
   namespace remote_control {                                           __NL__ \
                                                                        __NL__ \
      constexpr int first_entry_counter = __COUNTER__;                  __NL__ \
                                                                        __NL__ \
      REMOTE_CONTROL_DEFINITION(PREPARE_CHECK_SIGNATURES)               __NL__ \
                                                                        __NL__ \
      void read(uint8_t entry_id) {                                     __NL__ \
                                                                        __NL__ \
         constexpr int first_entry_counter_read = __COUNTER__;          __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(SWITCH_READ)                      __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      void send(uint8_t entry_id) {                                     __NL__ \
                                                                        __NL__ \
         constexpr int first_entry_counter_send = __COUNTER__;          __NL__ \
                                                                        __NL__ \
         switch(entry_id) {                                             __NL__ \
            REMOTE_CONTROL_DEFINITION(SWITCH_SEND)                      __NL__ \
         }                                                              __NL__ \
      }                                                                 __NL__ \
   } /* namespace remote_control */                                     __NL__ \
   } /* namespace plugin */                                             __NL__ \
   } /* namespace kaleidoscope */
         
      
      
