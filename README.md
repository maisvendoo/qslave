# QSlave: Modbus network emulator

![](https://habrastorage.org/webt/vi/dp/c9/vidpc9mtfkceseuta_4-v2kauc4.png)

## Introduction

This application is software emulator of Modbus network. It emulate several slave devices connected by virtual network. This network connected to the physical Modbus network.

![](https://habrastorage.org/webt/kh/em/u4/khemu4l4me16yhzldgxrspr1his.png)

Adapter USB RS485 required for connection QSlave with physical network.

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

## 2. Setup virtual network configuration

You need several configuration files in XML format for setup network configuration. You can see example configuration in directory [cfg/example](https://github.com/maisvendoo/qslave/tree/master/cfg/example). The list of virtual slaves is in file exmaple.net

```
<?xml version="1.0" encoding="UTF-8" ?>
<Config>
	<Slave>
		<!-- Slave description -->
		<Description>Traffic light</Description>
		<!-- Slave ID -->
		<id>1</id>
		<!-- Slave config file (without *.xml extension) -->
		<config>traffic-light</config>
	</Slave>
</Config>
```

Earch slave has own config file. You can see file traffic-light.xml for example of slave configuration

```
<?xml version="1.0" encoding="UTF-8" ?>
<Config>
	<!-- Coils list -->
	<Coil>
		<address>16</address>
		<description>Red signal</description>
		<value>0</value>
	</Coil>
	<Coil>
		<address>17</address>
		<description>Yellow signal</description>
		<value>0</value>
	</Coil>
	<Coil>
		<address>18</address>
		<description>Green signal</description>
		<value>0</value>
	</Coil>
	
	<!-- Discrete inputs list -->
	<DiscreteInput>
		<address>0</address>
		<description>Ready</description>
		<value>1</value>
	</DiscreteInput>
	
	<!-- Holding registers list -->
	<HoldingRegister>
		<address>5</address>
		<description>Signal activity time</description>
		<value>15</value>
	</HoldingRegister>
	
	<!-- Input registers list -->
	<InputRegister>
		<address>2</address>
		<description>Signals count</description>
		<value>3</value>
	</InputRegister>
</Config>
```

## 3. Load configuration and work with QSlave

1. Run QSlave
2. Load network configuration from *.net file (File -> Open config)

![](https://habrastorage.org/webt/nq/jf/su/nqjfsuqlcw0lq8rc0b1mzylyuaq.png)

3. Select device in device list. You can see all device data structures in application Windows

![](https://habrastorage.org/webt/hv/qb/ze/hvqbze3cdlop3xil1vapdie_nd0.png)

4. Plugin USB RS485 adapter to you PC USB port. Adapter must connected to physical Modbus RTU network.
5. Select connection parameters (port name, baudrate, etc.) and press connect button
6. You can change all values in tables of device memory (Coils, Holding registers, Discrete inputs and Input registers). You also can see changes in coils and holding registers, when master device send data to virtual device.



