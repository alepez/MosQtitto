# MosQtitto

> mos-cute-itto

An MQTT client for Qt/Qml based on mosquitto.

Tested with Linux and Android.

## Build mosquitto

 - Download from [official repository](https://github.com/eclipse/mosquitto)
 - Build with cmake

For Android:

```
git clone https://github.com/eclipse/mosquitto.git
cd mosquitto
mkdir _build_android
cmake -DANDROID_NDK=/opt/android-ndk/android-ndk-r10e -DANDROID_ABI="armeabi-v7a" -DANDROID_NDK_HOST_X64="YES"  -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-4.9" -DCMAKE_TOOLCHAIN_FILE="/opt/android-ndk/build/cmake/android.toolchain.cmake" -DWITH_TLS=OFF -DWITH_THREADING=OFF -DCMAKE_C_FLAGS='-fPIC' ..
```
