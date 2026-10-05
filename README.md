# My 2D Engine

[![Build](https://github.com/YOUR_USERNAME/my-2d-engine/actions/workflows/build.yml/badge.svg)](https://github.com/YOUR_USERNAME/my-2d-engine/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://github.com/libsdl-org/SDL)

A lightweight 2D game engine written from scratch in **C++17** on top of **SDL3**.

> 🚧 **Status:** Early development. Currently a working window with a proper game loop and delta time.

---

## ✨ Features

- ✅ **SDL3 backend** — cross-platform (Windows, Linux, macOS)
- ✅ **Fixed game loop** with delta time
- ✅ **Clean `Application` architecture** — Init / Run / Shutdown
- ✅ **CMake build system** with vendored dependencies via git submodules
- 🚧 Input system (in progress)
- 🚧 Sprite rendering (planned)
- 🚧 Text rendering (planned)
- 🚧 Audio (planned)
- 🚧 Entity-Component-System (planned)
- 🚧 Scene editor (planned)

---

## 🚀 Building

### Prerequisites

- **CMake** 3.16 or newer
- **C++17** compiler — MSVC 2019+, GCC 9+, or Clang 10+
- **Git** (with submodule support)

### Steps

```bash
# Clone with submodules (SDL3 will be pulled automatically)
git clone --recursive https://github.com/YOUR_USERNAME/my-2d-engine.git
cd my-2d-engine

# Configure and build
cmake -S . -B build
cmake --build build
```

Run the executable:

- **Windows:** `build\Debug\2DEngine.exe`
- **Linux/macOS:** `./build/2DEngine`

### If you forgot `--recursive`

```bash
git submodule update --init --recursive
```

---

## 📁 Project Structure

```
my-2d-engine/
├── src/
│   ├── core/
│   │   ├── Application.h      # Main app class (window, loop, lifecycle)
│   │   └── Application.cpp
│   └── main.cpp               # Entry point
├── vendored/
│   └── SDL/                   # SDL3 (git submodule)
├── .github/
│   └── workflows/             # CI configuration
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## 🗺 Roadmap

- [x] Window creation and game loop
- [x] Delta time calculation
- [ ] Input system (keyboard, mouse)
- [ ] Texture loading and sprite rendering (SDL_image)
- [ ] Text rendering (SDL_ttf)
- [ ] Audio playback (SDL_mixer)
- [ ] Entity-Component-System
- [ ] Simple scene editor (ImGui)
- [ ] First demo game

---

## 🤝 Contributing

This is a personal learning project, but feedback and suggestions are welcome!
Open an issue if you spot a bug or have an idea.

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgements

- [SDL3](https://github.com/libsdl-org/SDL) — the multimedia layer this engine is built on.