# ngpm Design Manifesto

## Notice

I use this service to replace upower. If you run KDE or solid based applications
this is probably not what you want to do. Please know how your system power
managment works before enabling `ngpmd` service and avoid multiple power
managers simultaneous run.

## How does ngpmd differ from upower?

ngpm avoids custom enumeration of power sources, it is meant to handle mostly
single battery scenario. `ngpmd` controls merely simple status and capacity
check. `ngpm` client and `ngpm.h` library can return status and capacity by
reading sysfs, again, no fancy API.

No device abstraction layer, no DBus dependency, and no event broadcasting.
`ngpmd` runs a single-threaded main loop pooling sysfs battery data using
`ngpm.h`. So both service and client pool from the same file - kernel is already
designed for shared access to sysfs files.

`ngpm.h` simplifies status response to 1 (charging) and 0 (discharging) and both
`battery_status` and `battery_capacity` return an `int8_t` with possbile `-1`
and `-2` error values.

So in short terms, ngpm is dead simple and is not compatible with KDE. It may
make sens though if your operating system is closely tight with glib.

## Pooling instead of monitoring

`ngpmd` battery check happends at fixed interval using `g_timeout_add_seconds()`
initialized by `DELAY_IN_SEC` preprpcessor option. You may ask: why not file
monitoring with `g_file_monitor()`?

- sysfs does not guaranty change events. i.m. `inotify` or `kqueue` may not
  behave consistently. Fighting this will require device abstraction layer, and
  this is what we want to avoid.

- Polling every 60-180 seconds is often cheaper and without accuracy losses.

- Less complexity - ngpm is intentionally a simple tool.

Theare are also potentially negative conseqences:

- With long intervals rapid state update may prompt unreliable. For this reason
  `ngpmd` does not bother to track exact events like: connected, disconnected.

## Cycle of ngpmd

1. If power supply is not connected do:
  1. If capacity <= `CRIT_THRESHOLD` system suspension.
  2. If state changed do:
    1. If capacity <= `THRESHOLD` display GTK 3 dialog or print message to stderr.
