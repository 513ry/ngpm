# ngpm

ngpm is a very simple user-space glib battery status monitor for Linux, BSD, and
Darvin with optional dependency on GTK 3 for alerts.

Please read [Design Manifesto](DESGIN.md) before installation.

## Installation

> [!CAUTION]
> I been not testing this version on NetBSD yet, battery status is most likely
> to be wrong if compiles.

Compilation should detect one of possible targets: FreeBSD, NetBSD, or Linux. To
compile and install the source run:

``` sh
bmake
bmake install
```

To compile ngpm with GTK 3 dialogs:

``` sh
bmake install -D WITH_GTK
```

> [!TIP]
> On NetBsd bmake is just [make](https://www.crufty.net/help/sjg/make_1.htm)

If you are a careful BSD user your probably want your `DESTDIR`:

``` sh
DESTDIR=/tmp bmake install
DESTDIR=/tmp bmake run-service
```

Additionally you may want to change your battery id, although it's not relevant
on FreeBSD as `hw.acpi.battery.life` will return combined capacity of all
available batteries.

``` sh
BATTERY=1 bmake
```

For more installation options check help command:

``` sh
bmake help
```

## Usage

After compilation you get two executables `ngpm` and `ngpmd`.

`ngpmd` should be installed as a autostart .desktop file and enabled at xsession
start.

`ngpm` command can help retrieve basic information about battery and ngpm
configuration. For example to get current battery capacity percentage go:

``` sh
ngpm -c
```

The data retrived will be capt according to the delay set by the `DELAY_IN_SEC`
preprpcessor definition. Another values to watch out for before compilation is
`THRESHOLD` and `CRIT_THRESHOLD`. You can list those values using client:

``` sh
ngpm -l
```

ngpm assumes you don't change those values often so changing them requires
recompilation.

To learn more about ngpm client use the help option:

``` sh
ngpm -h
```

## Contribute

To contribute:

1. Fork this repo
2. Make your changes on a separate branch
3. Apply your feature branch for merge
4. Thanks for contributing 🎊

## License

ngpm is a free software licensed under ISC suplementary to Berne, UCC, or TRIPS
copyright conventions.
