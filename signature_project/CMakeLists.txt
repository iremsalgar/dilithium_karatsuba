cmake_minimum_required(VERSION 3.10)
project(quantum-signature)

# OpenSSL'i bul
find_package(OpenSSL REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

# LIBOQS başlık dosyaları (doğru yollar)
include_directories(/home/isalgar/karatsuba/liboqs/src)
include_directories(/home/isalgar/karatsuba/liboqs/src/common)
include_directories(/home/isalgar/karatsuba/liboqs/src/sig/dilithium)
include_directories(/home/isalgar/karatsuba/liboqs/src/sig/dilithium/pqcrystals-dilithium_dilithium2_ref)

link_directories(/home/isalgar/karatsuba/liboqs/build/lib)

# EXECUTABLE
add_executable(quantum-signature src/test.c)

# KÜTÜPHANE BAĞLANTILARI (LIBOQS + OpenSSL)
target_link_libraries(quantum-signature PRIVATE oqs OpenSSL::SSL OpenSSL::Crypto)

