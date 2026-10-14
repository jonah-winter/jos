cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE
  "${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake"
  CACHE FILEPATH "Cross-compilation toolchain"
)

project(jos LANGUAGES CXX)

set(OUT "${CMAKE_BINARY_DIR}/exe")
set(IMAGE "${OUT}/os.img")
set(DISK "${OUT}/disk.img")

file(MAKE_DIRECTORY "${OUT}")

find_program(NASM nasm REQUIRED)
find_program(QEMU qemu-system-i386 REQUIRED)

add_custom_command(
  OUTPUT "${OUT}/entry.o" "${OUT}/io.o"
  COMMAND "${NASM}" -f elf32
    "${CMAKE_SOURCE_DIR}/src/boot/kernel_entry.asm"
    -o "${OUT}/entry.o"
  COMMAND "${NASM}" -f elf32
    "${CMAKE_SOURCE_DIR}/src/boot/io_ports.asm"
    -o "${OUT}/io.o"
  DEPENDS
    src/boot/kernel_entry.asm
    src/boot/io_ports.asm
  VERBATIM
)

add_executable(kernel
  src/kernel/kernel.cpp
  src/kernel/vga.cpp
  src/kernel/keyboard.cpp
  src/kernel/ide.cpp
  src/libj/memory.cpp
  "${OUT}/entry.o"
  "${OUT}/io.o"
)

set_source_files_properties(
  "${OUT}/entry.o"
  "${OUT}/io.o"
  PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE
)

target_include_directories(kernel PRIVATE src/kernel src)

target_compile_options(kernel PRIVATE
  -m32
  -ffreestanding
  -fno-pie
  -fno-pic
  -fno-exceptions
  -fno-rtti
  -fno-use-cxa-atexit
)

target_link_options(kernel PRIVATE
  -m32
  -nostdlib
  "-T${CMAKE_SOURCE_DIR}/linker/linker.ld"
  -Wl,--build-id=none
)

set_target_properties(kernel PROPERTIES
  SUFFIX ".elf"
  RUNTIME_OUTPUT_DIRECTORY "${OUT}"
)

set(KERNEL_SECTORS_INC "${OUT}/kernel_sectors.inc")
add_custom_command(
  OUTPUT "${KERNEL_SECTORS_INC}"
  COMMAND "${CMAKE_COMMAND}"
    "-DKERNEL_BIN=${OUT}/kernel.bin"
    "-DOUTPUT_FILE=${KERNEL_SECTORS_INC}"
    -P "${CMAKE_SOURCE_DIR}/cmake/GenerateKernelSectors.cmake"
  DEPENDS
    "${OUT}/kernel.bin"
    "${CMAKE_SOURCE_DIR}/cmake/GenerateKernelSectors.cmake"
  VERBATIM
)

add_custom_command(
  OUTPUT "${OUT}/boot.bin"
  COMMAND "${NASM}" -I "${OUT}/" -f bin
    "${CMAKE_SOURCE_DIR}/src/boot/boot.asm"
    -o "${OUT}/boot.bin"
  DEPENDS
    src/boot/boot.asm
    "${KERNEL_SECTORS_INC}"
  VERBATIM
)

add_custom_command(
  OUTPUT "${OUT}/kernel.bin"
  COMMAND i686-elf-objcopy -O binary
    "$<TARGET_FILE:kernel>"
    "${OUT}/kernel.bin"
  DEPENDS kernel
  VERBATIM
)

add_custom_command(
  OUTPUT "${IMAGE}"
  COMMAND dd if=/dev/zero "of=${IMAGE}" bs=512 count=2880
  COMMAND dd "if=${OUT}/boot.bin" "of=${IMAGE}" conv=notrunc
  COMMAND dd "if=${OUT}/kernel.bin" "of=${IMAGE}" bs=512 seek=1 conv=notrunc
  DEPENDS
    "${OUT}/boot.bin"
    "${OUT}/kernel.bin"
  VERBATIM
)
add_custom_target(os ALL DEPENDS "${IMAGE}")
add_custom_target(run
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUT}"
  COMMAND sh -c
    "if [ ! -f '${DISK}' ]; then dd if=/dev/zero of='${DISK}' bs=512 count=2048; fi"
  COMMAND "${QEMU}"
    -drive "format=raw,file=${IMAGE},if=ide,index=0"
    -drive "format=raw,file=${DISK},if=ide,index=1"
    -device "isa-debug-exit,iobase=0x501,iosize=0x04"
  DEPENDS os
  USES_TERMINAL
  VERBATIM
)
