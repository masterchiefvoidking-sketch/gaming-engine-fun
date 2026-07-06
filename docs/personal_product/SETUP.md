# Setup Guide

## Requirements

- CMake 3.24+
- C++20 compiler (GCC 11+, Clang 14+, MSVC 2022)
- Vulkan SDK (Windows builds)
- Modern Chromium browser (Web builds)

## Build

```bash
cmake -B build -DEVE_BUILD_TESTS=ON -DEVE_BUILD_PERSONAL_PRODUCT_CUT=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Launch Editor

```bash
./build/apps/creator_studio/eve-creator-studio Game/ApartmentLifeDemo
```

## Launch Demo

```bash
./build/apps/apartment_life_demo/eve-apartment-life-demo
```

## Personal Product Cut

```bash
./build/apps/personal_product_cut/eve-personal-product-cut data Game/ApartmentLifeDemo Releases
```
