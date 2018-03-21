# OSM Railway Stats

This repository contains a tool to create statistics about railway data in OpenStreetMap.

## Experimental!

This tool is experimental and can/should be used as a base for your own
experiments.


## Dependencies

This software uses the [Osmium library](https://github.com/osmcode/libosmium)
for everything related with reading OSM data.


## License

See the [LICENSE](LICENSE) file for the license of this program.


## How it works

This program reads a full history planet (or a spatial extract of it). The
planet must be sorted by object type (node, way, relation), ID and version –
that's default for files provided by planet.openstreetmap.org.

If two subsequent objects in the file have the same ID, this program compares
the value of the `building=*` tag. It can count the number of changes from
`building!=yes` to `building=yes` and write an OSM file which can be used by
revert tools to revert these changes.

Memory requirements are low because no data is cached apart from reading small
parts of the input file one after another. Most of the time is spent on reading
the input file.


## Dependencies

* C++11 compiler
* libosmium (`libosmium-dev`) and all its [important
* [dependencies](http://osmcode.org/libosmium/manual.html#dependencies)
* CMake (`cmake`)

You can install libosmium either using your package manager or just cloned from
its Github repository to any location on your disk. Take care to use libosmium
v2.x not the old Osmium v1.x!


## Building

```sh
mkdir build
cd build
cmake ..
make
```

