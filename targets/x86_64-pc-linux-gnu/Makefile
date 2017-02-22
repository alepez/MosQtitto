.PHONY: default
default: all

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(DIST_DIR)

.PHONY: all
all: app

.PHONY: app
app: $(DIST_DIR)/bin/MosQtitto

$(DIST_DIR)/bin/MosQtitto: $(BUILD_DIR)/MosQtitto
	@mkdir -p $(shell dirname $@)
	@cp $(BUILD_DIR)/MosQtitto $(DIST_DIR)/bin/MosQtitto

$(BUILD_DIR)/MosQtitto: $(BUILD_DIR)/Makefile $(shell find $(QML_DIR)) $(shell find $(SRC_DIR))
	@mkdir -p $(shell dirname $@)
	@cd $(shell dirname $@) && make

$(BUILD_DIR)/Makefile: $(PRO_FILE)
	@mkdir -p $(shell dirname $@)
	@cd $(shell dirname $@) && TARGET=$(TARGET) qtchooser -qt=5 -run-tool=qmake $(PRO_FILE)

.PHONY: run
run: app
	LD_LIBRARY_PATH=$(DIST_LIB_DIR) $(DIST_DIR)/bin/MosQtitto
