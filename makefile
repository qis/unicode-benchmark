MAKEFLAGS += --no-print-directory

CC	:= clang
CXX	:= clang++
CMAKE	:= CC=$(CC) CXX=$(CXX) CPPFLAGS="-fdiagnostics-absolute-paths -fcolor-diagnostics" cmake -GNinja
CMAKE	+= -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
CMAKE	+= -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/opt/vcpkg/scripts/toolchains/linux.cmake
CMAKE	+= -DVCPKG_TARGET_TRIPLET=x64-linux
CMAKE	+= -DCMAKE_INSTALL_PREFIX=$(PWD)
CMAKE	+= -DCMAKE_VERBOSE_MAKEFILE=OFF
PROJECT	!= grep "^project" CMakeLists.txt | cut -c9- | cut -d " " -f1

all: debug release

debug: build/llvm/debug/CMakeCache.txt
	@cmake --build build/llvm/debug

release: build/llvm/release/CMakeCache.txt
	@cmake --build build/llvm/release

run: build/llvm/release/CMakeCache.txt
	@cmake --build build/llvm/release --target run

gdb: build/llvm/debug/CMakeCache.txt
	@cmake --build build/llvm/debug --target $(PROJECT)
	@gdb -q -return-child-result -ex "run" -ex "thread apply all bt" -ex "quit" --args \
	  build/llvm/debug/$(PROJECT)

test: build/llvm/debug/CMakeCache.txt
	@cmake --build build/llvm/debug --target $(PROJECT)
	@cd build/llvm/debug && ctest

install: build/llvm/release/CMakeCache.txt
	@cmake --build build/llvm/release --target install

build/llvm/debug/CMakeCache.txt: build/llvm/debug CMakeLists.txt
	@mkdir -p build/llvm/debug; cd build/llvm/debug && \
	  $(CMAKE) -DCMAKE_BUILD_TYPE=Debug $(PWD)

build/llvm/release/CMakeCache.txt: build/llvm/release CMakeLists.txt
	@mkdir -p build/llvm/release; cd build/llvm/release && \
	  $(CMAKE) -DCMAKE_BUILD_TYPE=Release $(PWD)

build/llvm/debug:
	@mkdir -p $@

build/llvm/release:
	@mkdir -p $@

uninstall:
	@rm -rf bin

format:
	@clang-format -i `find src -type f -name '*.hpp' -or -name '*.cpp'`

clean:
	@rm -rf build/llvm

.PHONY: all debug release run gdb test install uninstall format clean
