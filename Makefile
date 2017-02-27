SRC_DIR := $(abspath ./src)
QML_DIR := $(abspath ./qml)
BUILD_DIR := $(abspath ./_build)
DIST_DIR := $(abspath ./_dist)

PROJECT_DIR := $(abspath ./)
PRO_FILE ?= $(abspath $(PROJECT_DIR)/MosQtitto.pro)

export PROJECT_DIR
export PRO_FILE
export SRC_DIR
export QML_DIR

.PHONY: default
default: x86_64-pc-linux-gnu

.PHONY: all
all: x86_64-pc-linux-gnu android_armv7

.PHONY: clean
clean:
	@rm -fr $(BUILD_DIR) $(DIST_DIR)

################################################################################

.PHONY: x86_64-pc-linux-gnu/run
x86_64-pc-linux-gnu/run:
	$(MAKE) -C targets/x86_64-pc-linux-gnu BUILD_DIR=$(BUILD_DIR)/x86_64-pc-linux-gnu DIST_DIR=$(DIST_DIR)/x86_64-pc-linux-gnu run

.PHONY: x86_64-pc-linux-gnu
x86_64-pc-linux-gnu:
	$(MAKE) -C targets/x86_64-pc-linux-gnu BUILD_DIR=$(BUILD_DIR)/x86_64-pc-linux-gnu DIST_DIR=$(DIST_DIR)/x86_64-pc-linux-gnu

################################################################################

.PHONY: android_armv7/run
android_armv7/run:
	$(MAKE) -C targets/android_armv7 BUILD_DIR=$(BUILD_DIR)/android_armv7 DIST_DIR=$(DIST_DIR)/android_armv7 run

.PHONY: android_armv7
android_armv7:
	$(MAKE) -C targets/android_armv7 BUILD_DIR=$(BUILD_DIR)/android_armv7 DIST_DIR=$(DIST_DIR)/android_armv7
