# Hextontiler

## Purpose
Construction of hexagonal maps using Gravenguild's Hexton Tiles

![](gifs/sample_tile_insertion)

## Compilation (Linux Debian)
Make sure you have the following dependencies installed
```
sudo apt -y install qt5-default qtmultimedia5-dev libqt5multimedia5-plugins libqt5gamepad5-dev build-essential cmake libboost-all-dev libglm-dev
```

Use the following compilation directives
```
mkdir build
cd build
qmake ../hextontiler.pro
make -j9
```
