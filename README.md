# Kaleidoscope-RemoteControl

Control you keyboard through serial communication with the host.

Literally anything that can be triggered programatically from within the sketch can now
be controlled from the host via the focus protocol.

## A Simple Usage Example

Before setting up your sketch, checkout the `Kaleidoscope-RemoteControl` from
git to your favorite plugin location (a location where Arduino picks up the 
plugin) or register it from within you Arduino IDE.

Next, to enable remote controlling, include the `Kaleidoscope-RemoteControl.h`
header at the beginning of your sketch

```cpp
#include "Kaleidoscope-RemoteControl.h"
```

Register the remote control plugin 

```cpp
KALEIDOSCOPE_INIT_PLUGINS(
   ...other plugins...
   RemoteControl
)
```

Almost done. Now you only need to setup what is supposed to be remote controlled.
This is done by defining some magic macros at any place in your sketch after the include directives.

Let's look at an example. Say you want to do the following via remote control: 

* activate a specific layer
* switch to a specific LED mode
* get the current LED mode
* reboot the keyboard

The corresponding remote control setup would read as follows.

```cpp
#define REMOTE_CONTROL(OP) \
   OBJECT_METHOD(OP, 0, ::Layer, activate) \
   OBJECT_METHOD(OP, 1, ::LEDControl, set_mode) \
   OBJECT_METHOD(OP, 2, ::LEDControl, get_mode_index) \
   OBJECT_METHOD(OP, 3, ::KeyboardHardware, rebootBootloader)
   
REMOTE_CONTROL_INIT(REMOTE_CONTROL) 
```

**Important:** The continuation markers `\` at the end of each line of the definition of function macro `REMOTE_CONTROL` are required for the pre-processor to read the macro definition as a single line.

Now you can use the `focus-test` program to test your setup. The following 
are the commands to be entered in a console window. The `focus-test` command
resides in the `bin` folder of the `Kaleidoscope` repository.

```
# Activate layer 3
#
focus-test remote_control 0 3

# Activate LED mode 2
#
focus-test remote_control 1 2

# Retreive the current LED mode
#
focus-test remote_control 2

# Reboot the keyboard
#
focus-test remote_control 3
```

## General Usage

In the above example but also in general, features to be controlled are registered with the remote control system via the two macros `REMOTE_CONTROL_INIT` and `REMOTE_CONTROL`. While the former
comes pre-defined by the plugin, the latter is responsible for the actual configuration.

The general syntax for its definition is as follows
```cpp
#define REMOTE_CONTROL(OP) \
   <COMMAND>(OP, <ID>, <ARGS...>) \
   <COMMAND>(OP, <ID>, <ARGS...>) \
   ...
```

Here `<COMMAND>` must be replaced with one of the remote control commands explained below. 
`<ID>` must be replaced with the request ID. Please make sure to let request IDs start from zero and do not
use the same request ID twice for the same command type. Every command has
its own set of arguments so you have to replace `<ARGS...>` with what is required by the different commands.

The invokation of the `focus-test` command works as follows.

```
focus-test remote_control <ID> [values...]
```

## Remote Control via Instance Methods and Global Functions

When applied to instance methods or global functions, the remote control can forward
any number of parameters to the keyboard and use them as call parameters (including zero number of parameters). Only if the method or function has a non-void return value, it is passed back to the host.

## Command Information

The `focus-test` tool can be used to retreive information about possible remote controls.
Call the tool with `remote_control.help` as argument.

A possible output might look as follows:

```
> focus-test remote_control.help
0: [uint8_t] -> uint8_t
1: uint8_t, uint16_t -> void
2: void -> uint8_t
3: void -> void
```
This reads as 
```
<ID>: [input value(s)] -> [output value]
```
A single input value that is written in square brackets means that it can be ommited in a request. This applies only to commands that are specified as `DIRECT_ACCESS` (see explanation below). For such requests, if the input value is ommited, only the current value of the affected global variable is retreived. Otherwise, the previous value is send from the keyboard to the host. This allows for toggling a value.

## Requests

Every command is assigned a unique request ID (the second parameter to the command function macro).

## Commands

The following commands are available and can be used to define a processing
function macro `REMOTE_CONTROL` that is passed to the pre-defined `REMOTE_CONTROL_INIT`
macro function to initialize remote controlling.

*DIRECT_ACCESS
*OBJECT_METHOD
*GLOBAL_FUNCTION

### `DIRECT_ACCESS`

Passes a value (optional) to the keyboard and assigns it to a global instance. The former value of the global instance is returned to the host.
```
DIRECT_ACCESS(OP, ID, INSTANCE)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is affected.
```

```
focus-test remote_control <ID> [value]
```

### `OBJECT_METHOD`

Passes a set of values to the keyboard and uses them as call parameters of a class instance method. The return value of the method (if non-void) is passed back to the host.

```
OBJECT_METHOD(OP, ID, INSTANCE, METHOD)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is affected.
METHOD: The method to be called.
```

```
focus-test remote_control.set <ID> [values...]
```

### `GLOBAL_FUNCTION`

Passes a set of values to the keyboard and calls a global function with them as parameters.

```
GLOBAL_FUNCTION(OP, ID, FUNCTION)

OP: Used internally.
ID: The request id.
FUNCTION: The global function to be called.
```

```
focus-test remote_control.set <ID> [values...]
```

## Application Ideas and Examples

**You have more ideas about what to remote control? Just let me know and I can add your ideas to this list to inspire other people.**

### Signal EMail Arrival

Say, you want the whole keyboard to switch to a specific LED mode that announces arrival of new EMail. Given that you have a scripting interface in your EMail client, you could let it emit a `focus-test` call that triggers the LED mode switching on the keyboard. After you read your EMail, you could let the mail client signal via another `focus-test` call that all email was read (if it is capable of doing this). Alternatively, you could switch manually to the original LED mode on the keyboard.
