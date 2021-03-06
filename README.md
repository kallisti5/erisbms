# ErisBMS

A battery management system to help regulate chaos.

# Supported Chemestries

* LiPo
* LiIon

# Usage

The sources need to be adjusted to your running environment,
this no pre-compiled binaries are available.

## Compiling

``platformio run``

## Uploading to Arduino

``platformio run -t upload``

# High level design

* **LiIon:** V_MAX = 4.2, V_MIN = 3.0, TEMP_MAX = 60

* Cells with voltage > V_MAX have their dump relays closed to bleed
  off excess capacity.
* Any cell with voltage < V_MIN results in the main output being
  disabled until voltage is re-applied 
* Extra: Any cell temp over TEMP_MAX C is disabled.

# DISCLAIMER

The work in this repository comes with NO warrany.
LiPo and LiIon batterys are extremely dangerious if mistreated.

By compiling the code in this repository, you agree to hold
authors at no fault in the event of any damages.

# Copyright

Copyright, 2016-2017 Alexander von Gluck IV. All rights reserved.
Released under the terms of the MIT license.
