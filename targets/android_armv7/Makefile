PROJECT_NAME = MosQtitto

# QT_ROOT must contain bin include jar lib mkspecs plugins qml src translations
# Normally it is located in /opt/Qt/5.8/android_armv7

QT_ROOT ?= /opt/Qt/5.8/android_armv7

# This is named by Project
DEPLOYMENT_SETTINGS_JSON = ${BUILD_DIR}/android-lib$(PROJECT_NAME).so-deployment-settings.json

# ANDROID_HOME Must contain
# add-ons build-tools docs extras platforms platform-tools ... and other files
ANDROID_HOME ?= /opt/android/sdk

export ANDROID_HOME
export ANDROID_SDK_ROOT = $(ANDROID_HOME)

# ANDROID_NDK_ROOT Must contain
# platform ndk-depends ... and other files
export ANDROID_NDK_ROOT=/opt/android-ndk/android-ndk-r10e

export ANDROID_NDK_HOST=linux-x86_64
export ANDROID_NDK_PLATFORM=android-17
export ANDROID_NDK_TOOLCHAIN_PREFIX=arm-linux-androideabi
export ANDROID_NDK_VERSION=4.9
export ANDROID_NDK_TOOLS_PREFIX=arm-linux-androideabi

# qmake and androiddeployqt are normally found under QT_ROOT
QMAKE = $(QT_ROOT)/bin/qmake
ANDROIDDEPLOYQT = $(QT_ROOT)/bin/androiddeployqt

.PHONY: default
default: all

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(DIST_DIR)

.PHONY: all
all: apk

.PHONY: apk
apk: $(DIST_DIR)/build/outputs/apk/android_armv7-debug.apk

$(DIST_DIR)/build/outputs/apk/android_armv7-debug.apk: $(BUILD_DIR) $(DIST_DIR)
	cd $(BUILD_DIR) && \
		$(QMAKE) $(PRO_FILE) -spec android-g++ && \
		$(MAKE) qmake_all && \
		$(MAKE) && \
		$(MAKE) INSTALL_ROOT=$(DIST_DIR) install && \
		$(ANDROIDDEPLOYQT) --input $(DEPLOYMENT_SETTINGS_JSON) \
		--output $(DIST_DIR) --verbose --deployment bundled \
		--gradle

.PHONY: run
run: install
	adb shell am start -a android.intent.action.MAIN -n net.pezzato.myhome/org.qtproject.qt5.android.bindings.QtActivity

.PHONY: install
install: apk
	adb install -r $(DIST_DIR)/build/outputs/apk/android_armv7-debug.apk

$(BUILD_DIR):
	@mkdir -p $@

$(DIST_DIR):
	@mkdir -p $@
