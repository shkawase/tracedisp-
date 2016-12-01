# tracedisp++

tracedisp alternative with extensibility

## Description

``tracedisp++`` displays the beam trajectory in the SAMURAI magnet.


## Requirement

- ROOT 5.34
- yaml-cpp 0.3.0 (not 0.5.x)

## How to Compile

```sh
make
```

## Command Line Options

### -h

Display usage.

### -o

Specifies output file.

### -g

Specifies geometry configuration file.

### -m

Specifies magnet configuration file.


## ToDo

* organize sources
* enable to set flexible end conditions for tracing
* implement 3D tracing
    * with use of TGeoManager in ROOT?
* bug fix ...

