# ngpm Design Manifesto

## What Problem does it solve?

I use this service as a lightweight alternative to upower on certain graphical
desktop environments such as i3 where only basic battery status and capacity
information are needed. If you run KDE or solid based applications this is
probably not what you want to do. Please know how your system power managment
works before enabling `ngpmd` service and avoid multiple power managers
simultaneous run.

## Platform support

ngpm queries the operating system's native power management interface rather
than introducing its own abstraction:

- Linux uses sysfs,
- FreeBSD uses its native ACPI interface,
- NetBSD uses envsys.

## How does ngpmd differ from upower on Linux?

ngpm avoids custom enumeration of power sources, it is meant to handle single
battery systems exposing a single battery through sysfs. `ngpmd` periodically
reads battery status and capacity. Both the client utility and daemon use
ngpm.h, which reads battery information directly from sysfs.

No device abstraction layer, no DBus dependency, and no event broadcasting.
`ngpmd` runs a single-threaded delta delayed loop polling sysfs battery data
using `ngpm.h` API. So both service and client poll from the same file.

> [!NOTE]
> sysfs attributes are read-only kernel interfaces and are intended to be safely
> read concurrently by multiple processes.

`ngpm.h` simplifies status response to 1 (charging) and 0 (discharging) and both
`battery_status` and `battery_capacity` return an `int8_t` with possbile `-1`
and `-2` error values.

So in short terms, ngpm is dead simple, is not intended for critical
environments, and can potentially crash some KDE software. It may make sense on
lightweight GLib-based desktop environments that do not depend on UPower's D-Bus
interfaces.

## Polling instead of monitoring

`ngpmd` battery check happends at fixed interval using `g_timeout_add_seconds()`
initialized by `DELAY_IN_SEC` preprpcessor option. You may ask: why not file
monitoring with `g_file_monitor()`?

- `inotify` or `kqueue` may not behave consistently when relying on system
  events. Fighting this will require device abstraction layer, and this is what
  we want to avoid.

- Polling every 60–180 seconds has negligible overhead while keeping the
  implementation simple and portable.

Theare are also potentially negative conseqences:

- State changes are detected only during the next polling interval. For this
  reason `ngpmd` does not bother to track exact events like: connected,
  disconnected.

## Model of ngpmd states

- 1. If power supply is not connected do:

  - 1. If capacity <= `CRIT_THRESHOLD` suspend the system.

  - 2. If state changed do:

    - 1. If capacity <= `THRESHOLD` display GTK 3 dialog or print message to stderr.
