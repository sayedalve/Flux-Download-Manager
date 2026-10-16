# SPDX-License-Identifier: GPL-3.0-or-later
# Flux Download Manager — Reproducible C++20 Build System

CXX       := g++
CXXFLAGS  := -std=c++20 -O2 -Wall -Wextra -Werror -pthread -Isrc
LDFLAGS   := -pthread /usr/lib/x86_64-linux-gnu/libsqlite3.so.0 -lssl -lcrypto

BUILD_DIR := build

HEADERS   := $(wildcard src/*/*.hpp)

CORE_SRCS := \
	src/common/path_sanitizer.cpp \
	src/common/logger.cpp \
	src/common/crypto_utils.cpp \
	src/storage/sqlite_store.cpp \
	src/brisk_engine/segment_scheduler.cpp \
	src/brisk_engine/http_client.cpp \
	src/brisk_engine/http_download_engine.cpp \
	src/engine_stream/nm3u8dl_re_adapter.cpp \
	src/core/download_controller.cpp \
	src/ipc/ipc_server.cpp \
	src/ui/win32_main_window.cpp

CORE_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/obj/%.o,$(CORE_SRCS))

.PHONY: all clean test

all: $(BUILD_DIR)/openidm $(BUILD_DIR)/openidm-nmh $(BUILD_DIR)/N_m3u8DL-RE $(BUILD_DIR)/openidm_unit_tests

$(BUILD_DIR)/obj/%.o: src/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/openidm: $(CORE_OBJS) src/app/main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) src/app/main.cpp -o $@ $(LDFLAGS)
	strip -s $@
	cp -f $@ $(BUILD_DIR)/flux

$(BUILD_DIR)/openidm-nmh: $(CORE_OBJS) src/nmh/main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) src/nmh/main.cpp -o $@ $(LDFLAGS)
	strip -s $@
	cp -f $@ $(BUILD_DIR)/flux-nmh

$(BUILD_DIR)/N_m3u8DL-RE: $(BUILD_DIR)/obj/brisk_engine/http_client.o src/worker_stream/nm3u8dl_re_worker.cpp $(HEADERS)
	@mkdir -p tools
	$(CXX) $(CXXFLAGS) $(BUILD_DIR)/obj/brisk_engine/http_client.o src/worker_stream/nm3u8dl_re_worker.cpp -o $@ -pthread -lssl -lcrypto
	strip -s $@
	cp -f $@ tools/N_m3u8DL-RE
	chmod +x tools/N_m3u8DL-RE

$(BUILD_DIR)/openidm_unit_tests: $(CORE_OBJS) tests/cpp/unit_tests.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) tests/cpp/unit_tests.cpp -o $@ $(LDFLAGS)
	cp -f $@ $(BUILD_DIR)/flux_unit_tests

test: all
	./$(BUILD_DIR)/flux_unit_tests
	node tests/test_extension_detection.js
	python3 tests/run_all_integration_tests.py

clean:
	find $(BUILD_DIR) -type f -delete 2>/dev/null || true
