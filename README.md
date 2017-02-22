# MosQtitto

> mos-cute-itto

An MQTT client for Qt/Qml based on mosquitto.

Tested with Linux and Android.

## Dependency: mosquitto library

 - Download from [official repository](https://github.com/eclipse/mosquitto)
 - Build with cmake

For Android:

```
git clone https://github.com/eclipse/mosquitto.git
cd mosquitto
mkdir _build_android
cd _build_android
cmake -DANDROID_NDK=/opt/android-ndk/android-ndk-r10e -DANDROID_ABI="armeabi-v7a" -DANDROID_NDK_HOST_X64="YES"  -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-4.9" -DCMAKE_TOOLCHAIN_FILE="/opt/android-ndk/build/cmake/android.toolchain.cmake" -DWITH_TLS=OFF -DWITH_THREADING=OFF -DCMAKE_C_FLAGS='-fPIC' ..
```

## Build

Before building, check dependencies path in these files:

 - `targets/android_armv7/Makefile`
 - `targets/x86_64-pc-linux-gnu/Makefile`
 - `MosQtitto.pro`

Build for Linux:

```
make x86_64-pc-linux-gnu
```

Run in linux:

```
make x86_64-pc-linux-gnu/run
```

Build for Android:

```
make android_armv7
```

Run in an Android device:

```
make android_armv7/run
```
