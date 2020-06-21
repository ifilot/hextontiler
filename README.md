# Hextontiler

## Purpose
Construction of hexagonal maps using Gravenguild's Hexton Tiles. The main idea is that you can design your game map before printing it, potentially saving on material and giving you a good impression of what your map would look like before assembling it in real-life.

![](https://raw.githubusercontent.com/ifilot/hextontiler/master/gifs/sample_tile_insertion.gif)

## Usage instructions

### Map modification

| Action                | Instruction                                    |
| --------------------- |------------------------------------------------|
| **Add** a new tile    | Hover over an empty cell and press **SHIFT+A** |
| **Remove** a tile     | Hover over a tile and press **DELETE**         |
| **Substitute** a tile | Hover over a tile and press **SHIFT+S**        |
| **Rotate** a tile     | Hover over a tile and press **SHIFT+R**        |

### Loading and saving
To save the current map, either press **CTRL+S** or go to `File > Save`. Maps are stored in a human-readible format with the `.htm` extension. To load a map from a file, either press **CTRL+O** or go to `File > Open`.

### Building Bill of Materials
If you want an overview how many tiles of which type is in your map, go to `Tools > Construct Bill of Materials`.

![](https://raw.githubusercontent.com/ifilot/hextontiler/master/gifs/sample_bill_of_materials.gif)

## Installation (Microsoft Windows)
User-friendly installers are made for Windows. You can find the installers on the [releases](https://github.com/ifilot/hextontiler/releases) page or download them directly using the links below
| Version | Download link |
| ------- | ------------- |
| 0.1.0.0 | [Download](https://github.com/ifilot/hextontiler/releases/download/0.1.0.0.alpha/hextontiler_0.1.0.0_installer_win64.exe) |

## Compilation (Linux Debian)
This is for advanced users who would like to run Hextontiler on Linux. These instructions are based on Linux Debian, but can be readily extended to different Linux branches. Make sure you have the following dependencies installed
```
sudo apt -y install qt5-default qtmultimedia5-dev libqt5multimedia5-plugins libqt5gamepad5-dev build-essential cmake libboost-all-dev libglm-dev
```

Clone the repository and use the following compilation directives to compile the program
```
git clone git@github.com:ifilot/hextontiler.git --depth 1
mkdir build
cd build
qmake ../hextontiler.pro
make -j9
```
