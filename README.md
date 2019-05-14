# Kaleidoscope-RemoteControl

Control you keyboard through serial communication with the host.

Literally anything that can be triggered programatically from within the sketch can now
be controlled from the host via the focus protocol.

## Usage Example

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
   ...
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
   OBJECT_SETTER(OP, 0, ::Layer, activate) \
   OBJECT_SETTER(OP, 1, ::LEDControl, set_mode) \
   OBJECT_GETTER(OP, 2, ::LEDControl, get_mode_index) \
   OBJECT_GETTER(OP, 3, ::KeyboardHardware, rebootBootloader)
   
REMOTE_CONTROL_INIT(REMOTE_CONTROL) 
```

Now you can use the `focus-test` program to test your setup. The following 
are the commands to be entered in a console window. The `focus-test` command
resides in the `bin` folder of the `Kaleidoscope` repository.

```
# Activate layer 3
#
focus-test remote_control.set 0 3

# Activate LED mode 2
#
focus-test remote_control.set 1 2

# Retreive the current LED mode
#
focus-test remote_control.get 2

# Reboot the keyboard
#
focus-test remote_control.trigger 3
```

## General Usage

Like in the above example the specification of features to be controlled works
also in general via the two macros `REMOTE_CONTROL_INIT` and `REMOTE_CONTROL`. While the former
is pre-defined by the plugin, the latter is responsible for the actual configuration.

The general syntax for its definition is as follows
```cpp
#define REMOTE_CONTROL(OP) \
   <COMMAND>(OP, <ID>, <ARGS...>) \
   <COMMAND>(OP, <ID>, <ARGS...>) \
   ...
```

Here `<COMMAND>` must be replaced with one of the remote control commands explained below. 
`<ID>` is the request ID. Please make sure to let request IDs start from zero and do not
use the same request ID twice for the same command type. Every command has
its own set of arguments so you have to replace `<ARGS...>` with what is required by the different commands.

The invokation of the `focus-test` command works as follows.

```
# Getter commands
#
focus-test remote_control.get <ID>

# Setter commands
#
focus-test remote_control.set <ID> <value>

# Trigger commands
#
focus-test remote_control.trigger <ID>
```

## Command Classes

There are three classes of remote control commands.

### Getter Commands

Getter commands are those commands that transfer information to the host (from the keyboard's perspective) or get information from the keyboard (from the host's perspective). The corresponding commands on the keyboard side are `DIRECT_GET` and `OBJECT_GETTER` and `GLOBAL_GETTER`. See information about these commands further on in this document.

### Setter Commands

Setter commands are those commands that receive information from the host (from the keyboard's perspective) or transfers information to the keyboard (from the host's perspective). The corresponding commands on the keyboard side are `DIRECT_SET` and `OBJECT_SETTER` and `GLOBAL_SETTER`. See information about these commands further on in this document.

### Trigger Commands

Trigger commands trigger an action either by calling a global function or a method of a global class instance. The corresponding commands on the keyboard are `OBJECT_TRIGGER` and `GLOBAL_TRIGGER`.

## Requests

Every command is assigned a request ID (the second parameter to the command function macro).
Requests of disjoint type (Getter != Setter != Requests) can share an ID to be grouped.
The same ID cannot be used twice for for two commands of the same command class.

## Commands

The following commands are available and can be used to define a processing
function macro `REMOTE_CONTROL` that is passed to the pre-defined `REMOTE_CONTROL_INIT`
macro function to initialize remote controlling.

*DIRECT_SET
*DIRECT_GET
*OBJECT_SETTER
*OBJECT_GETTER
*GLOBAL_SETTER
*GLOBAL_GETTER
*OBJECT_TRIGGER
*GLOBAL_TRIGGER

### `DIRECT_SET`

Passes a value to the keyboard and assignes it to a global instance.
```
DIRECT_SET(OP, ID, INSTANCE)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is assigned a value.
```

```
focus-test remote_control.set <ID> <value>
```

### `DIRECT_GET`

Receives a value from the keyboard.

```
DIRECT_GET(OP, ID, INSTANCE)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance whose value is send to the host.
```

```
focus-test remote_control.get <ID>
```

### `OBJECT_SETTER`

Passes a value to the keyboard and assignes it to a global instance via a setter method.

```
OBJECT_SETTER(OP, ID, INSTANCE, SETTER)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is affected.
SETTER: The setter method to be called.
```

```
focus-test remote_control.set <ID> <value>
```

### `OBJECT_GETTER`

Retreives a value from the keyboard via a getter method invoked on a global instance.

```
OBJECT_GETTER(OP, ID, INSTANCE, GETTER)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is affected.
GETTER: The getter method to be called.
```

```
focus-test remote_control.get <ID>
```

### `GLOBAL_SETTER`

Passes a value to the keyboard and calls a function with it as parameter.

```
GLOBAL_SETTER(OP, ID, SETTER)

OP: Used internally.
ID: The request id.
SETTER: The setter function to be called.
```

```
focus-test remote_control.set <ID> <value>
```

### `GLOBAL_GETTER`

Retreives a value from the keyboard via a global getter function.

```
GLOBAL_GETTER(OP, ID, GETTER)

OP: Used internally.
ID: The request id.
GETTER: The getter function to be called.
```

```
focus-test remote_control.get <ID>
```

### `OBJECT_TRIGGER`

Triggers an action on the keyboard by calling a method of a global
class instance (with void arguments).

```
OBJECT_TRIGGER(OP, ID, INSTANCE, TRIGGER)

OP: Used internally.
ID: The request id.
INSTANCE: The global instance that is affected.
TRIGGER: The trigger method to be called.
```

```
focus-test remote_control.trigger <ID>
```

### `GLOBAL_TRIGGER`

Triggers an action on the keyboard by calling a global function (with void arguments).

```
GLOBAL_TRIGGER(OP, ID, TRIGGER)

OP: Used internally.
ID: The request id.
TRIGGER: The trigger function to be called.
```

```
focus-test remote_control.trigger <ID>
```
