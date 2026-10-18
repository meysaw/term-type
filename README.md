# TERM_TYPE

A terminal typing speed test, built with C++17, [FTXUI](https://github.com/ArthurSonzogni/FTXUI) and CMake.

A random passage is shown in the terminal. Type it as accurately and as fast as
you can. The 30 second timer starts on your first keystroke. When time runs out
(or you finish the passage) you get your accuracy, WPM and raw WPM.

## Requirements

- A C++17 compiler — Clang, GCC, or MSVC
- CMake 3.20 or newer
- Git, and a network connection for the first configure (unless FTXUI is
  already installed, see below)

FTXUI is downloaded automatically the first time you configure, so there is
nothing to install by hand and no submodule to initialise.

## Build

```bash
git clone https://github.com/meysaw/term-type.git
cd term-type
cmake -B build
cmake --build build
```

Run it:

```bash
./build/term-type
```

After editing `CMakeLists.txt`, just run `cmake --build build`; it re-configures
itself when needed.

### Build types

The commands above produce a `Release` build. For a debug build:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Sanitizers

To catch memory and undefined-behaviour bugs during development (GCC or Clang),
use a separate build directory:

```bash
cmake -B build-san -DCMAKE_BUILD_TYPE=Debug -DTERM_TYPE_SANITIZE=ON
cmake --build build-san
./build-san/term-type
```

Warnings (`-Wall -Wextra`, or `/W4` on MSVC) are always enabled for this
project's own code.

### Using a system FTXUI

If FTXUI is already installed and discoverable, CMake uses it and skips the
download. To force the pinned download instead, configure with:

```bash
cmake -B build -DTERM_TYPE_USE_SYSTEM_FTXUI=OFF
```

## Controls

| Key                     | Action                                                       |
| ----------------------- | ------------------------------------------------------------ |
| any printable ASCII key | type the next character (the timer starts on your first key) |
| `Backspace`             | delete the previous character                                |
| `R` or `Enter`          | on the results screen, restart with a new random passage     |
| `Esc`                   | quit                                                         |

Non-ASCII input (accents, emoji, ...) is ignored.

## Project layout

```
CMakeLists.txt   build definition
src/main.cpp     entire application
LICENSE          MIT
```

## License

MIT — see [LICENSE](LICENSE).
