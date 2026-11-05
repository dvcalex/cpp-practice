# cpp-codebase-boilerplate

A C++23 CMake starter code.

## Requirements

- A C++23 compiler
- CMake 3.26+
- A build tool for your chosen CMake generator (i.e., Make, Ninja, or Visual Studio)

## Build and run

```sh
cmake --preset debug
cmake --build --preset debug
./build/debug/bin/myproject

cmake --preset release
cmake --build --preset release
./build/release/bin/myproject
```

`release` uses `Release` optimization.

Use `optimized-with-symbols` for an optimized build **with debug information** in `build/optimized-with-symbols/`:

```sh
cmake --preset optimized-with-symbols
cmake --build --preset optimized-with-symbols
```

This preset uses CMake’s `RelWithDebInfo` configuration where optimizations and debug symbols are enabled.

Use `-DMYPROJECT_ENABLE_LOGGING=ON` when configuring to retain `DEBUG_LOG`.

Presets use CMake's default generator. To select another during CMake configure, do something like, `cmake --preset debug -G Ninja`.

## Using this for a new project

1. Make the repo using this template

   ```sh
   gh repo create my-new-thing --private --template dvcalex/cpp-codebase-boilerplate --clone
   ```

2. Find and rename instances of `myproject` or `MYPROJECT` to your actual project's name.

3. List new `.cpp` files in the `add_library` block of CMakeLists.txt.

## Logging

To keep diagnostic logging in an optimized build:

```sh
cmake --preset release -DMYPROJECT_ENABLE_LOGGING=ON
cmake --build --preset release
```

The option is cached; configure with `-DMYPROJECT_ENABLE_LOGGING=OFF` to disable it again.

`myproject::log::log`, `warn`, and `error` calls log to `stderr`.

# Debug Breaks

Include `<myproject/DebugBreak.hpp>` for `MYPROJECT_DEBUG_BREAK()`. It uses compiler intrinsics where available and falls back to `std::abort()`. It is active in all build configurations.

## Resources

If you want to copy files next to your program binaries in their build folders, create a `res/` directory in the project root and do CMake configure again.

CMake copies its contents to `bin/res/` on every build, including when only resources changed. Unchanged files are not recopied.

NOTE: Deleted source resources are NOT removed from the destination, so manual deletion would be required.

NOTE: Relative paths such as `res/some.txt` resolve from the working directory.
Run from the executable's directory when loading them:

```sh
cd build/debug/bin
./myproject
```

## Adding a dependency

Vendor dependencies under `vendor/`. Examples are included at the bottom of `CMakeLists.txt`. Mark raw third-party include directories as `SYSTEM` to suppress their warnings. Dependencies that compile C sources may also require adding `C` to the project's `LANGUAGES`.

Alternatively, use CMake's `FetchContent`.

## Adding tests

```cmake
include(CTest)
if(BUILD_TESTING)
    add_executable(tests tests/main.cpp)
    target_link_libraries(tests PRIVATE ${PROJECT_NAME}_lib)
    add_test(NAME tests COMMAND tests)
endif()
```

After adding your tests, configure and build like usual, then run:

```sh
ctest --test-dir build/debug -C Debug --output-on-failure
```
