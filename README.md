# obc_current
OBC repo where only clean, functional code is placed. 'obc' repo is deprecated to a code storage area.

## Setup

Packages required
```
sudo apt install build-essential libi2c-dev cmake
```

## Building and Running

Make sure to build on linux because drivers will be using linux things.

```
mkdir build && cmake ..
make
./obc_current
```
