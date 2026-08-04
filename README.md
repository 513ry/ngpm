# ngpm

ngpm is a very simple user-space GLib battery status monitor for Linux and BSD
with an optional dependency on GTK 3 for graphical alerts.

Please read [Design Manifesto](DESIGN.md) before installation.

## Installation

> [!CAUTION]
> I been not testing this version on NetBSD yet, battery status is most likely
> to be wrong if compiles.

Compilation should detect one of the possible targets: FreeBSD, NetBSD, or
Linux. To compile the source, run:

``` sh
bmake
```

To compile ngpm with GTK 3 dialogs:

``` sh
bmake -D WITH_GTK
```

After installation, `ngpmd` should start automatically in new sessions on most
POSIX systems:

``` sh
bmake install
```

If you are a careful BSD user looking for your `DESTDIR`:

``` sh
DESTDIR=/tmp bmake install
```

Additionally, you may want to change your battery ID (although this is not
relevant on FreeBSD, as `hw.acpi.battery.life` will return the combined capacity
of all available batteries):

``` sh
BATTERY=1 bmake
```

For more installation options check help command:

``` sh
bmake help
```

> [!TIP]
> On NetBSD bmake is just [make](https://www.crufty.net/help/sjg/make_1.htm)

## Usage

After compilation you get two executables `ngpm` and `ngpmd`.

`ngpmd` is not a system daemon - `ngpmd.desktop` file will be installed to
enable session application autostart.

The `ngpm` command can help retrieve basic information about the battery and
configuration. For example, to print the current battery capacity percentage,
run:

``` sh
ngpm -c
```

It's worth noting that non meta commands can be chained and execte in option set
defined order. The listed outputs will be separated be a new line character
('\n'):

``` sh
ngpm -cs
```

`ngpmd` comiplation constants can be listed with:

``` sh
ngpm -l
```

ngpm assumes you don't change those values often, so changing them requires
recompilation.

To learn more about `ngpm` client, use the help option:

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

ngpm is a free/open software licensed under ISC, suplementary to Berne, UCC, or
TRIPS copyright conventions.
