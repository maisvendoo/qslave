# QSlave: Modbus network emulator

![](https://habrastorage.org/webt/vi/dp/c9/vidpc9mtfkceseuta_4-v2kauc4.png)

## Introduction

This application is software emulator of Modbus network. It emulate several slave devices connected by virtual network. This network connected to the physical Modbus network.

![](https://habrastorage.org/webt/kh/em/u4/khemu4l4me16yhzldgxrspr1his.png)

## 1. Installation

### 1.1. Building from sources

1. Get sources from repository

```
$ mkdir qslave
$ cd qslave
$ git clone https://github.com/maisvendoo/qslave.git qslave
```

2. Make build directory

```
$ mkdir build
$ cd build
```

3. Generate Makefile

```
$ qmake ../qslave
```

4. Run make

```
$ make
```

### 1.2. Windows setup

Download last release from [release page](https://github.com/maisvendoo/qslave/releases). Run qslave-vX.Y.Z-setup.exe and folow installation instructions.

