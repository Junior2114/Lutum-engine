# My 2D Engine

[![Build](https://github.com/Junior2114/my-2d-engine/actions/workflows/build.yml/badge.svg)](https://github.com/Junior2114/my-2d-engine/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://github.com/libsdl-org/SDL)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](#)

A lightweight 2D game engine written from scratch in **C++17** on top of **SDL3**.

The goal of this project is to provide a clean, minimal, and educational foundation for 2D games — without hiding the internals behind layers of abstraction. Every subsystem (input, rendering, animation, and later ECS) is implemented explicitly so that the engine can be understood and extended by anyone reading the source.

---

## ✨ Features

- ✅ **Cross-platform** — Windows, Linux, macOS (SDL3 backend)
- ✅ **Clean architecture** — `Application`, `Renderer`, `Input`, `Texture`, `Animation`, `Animator`
- ✅ **Fixed game loop** with delta time
- ✅ **Keyboard and mouse input** with `down` / `pressed` / `released` states
- ✅ **Sprite rendering** with RAII-based `Texture` management
- ✅ **Sprite sheet animations** — frame-based, directional (4-way walk)
- ✅ **CMake build system** with vendored dependencies via git submodules
- 🚧 Entity-Component-System (planned)
- 🚧 Camera and tilemap (planned)
- 🚧 Scene editor (planned)
- 🚧 Audio (planned)

---

## 🎬 Demo

> _Coming soon — animated GIF of the player walking in four directions._

<!--
Once you have a GIF, replace this with:
![Demo](docs/demo.gif)
-->

---

## 🚀 Building

### Prerequisites

- **CMake** 3.16 or newer
- **C++17** compiler — MSVC 2019+, GCC 9+, or Clang 10+
- **Git** (with submodule support)

### Steps

```bash
# Clone with submodules (SDL3 and SDL3_image will be pulled automatically)
git clone --recursive https://github.com/Junior2114/my-2d-engine.git
cd my-2d-engine

# Configure and build
cmake -S . -B build
cmake --build build
```

### Running

- **Windows:** `build\Debug\2DEngine.exe`
- **Linux/macOS:** `./build/2DEngine`

> ⚠️ **Working directory matters.** The engine loads assets from `assets/` relative to the current working directory. Run the executable from the project root, or configure your IDE's working directory accordingly.

### Forgot `--recursive`?

If you cloned without submodules, run:

```bash
git submodule update --init --recursive
```

### Optional: disable unused codecs

SDL3_image enables many image formats by default (PNG, JPG, GIF, WebP, TIFF, SVG, ...). If you want a faster build and a smaller binary, disable the ones you don't need:

```bash
cmake -S . -B build \
  -DSDLIMAGE_AVIF=OFF \
  -DSDLIMAGE_JXL=OFF \
  -DSDLIMAGE_TIFF=OFF \
  -DSDLIMAGE_WEBP=OFF
```

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move up |
| `S` / `↓` | Move down |
| `A` / `←` | Move left |
| `D` / `→` | Move right |
| `Esc` | Quit |

---

## 📁 Project Structure

```
my-2d-engine/
├── assets/
│   └── textures/              # Sprite sheets and textures
│       └── player_sheet.png   # 4 directions × 4 frames, 32×32 each
├── src/
│   ├── core/
│   │   ├── Application.h/.cpp # Main app class: window, loop, lifecycle
│   │   └── Input.h/.cpp       # Keyboard and mouse input
│   ├── graphics/
│   │   ├── Renderer.h/.cpp    # Wrapper over SDL_Renderer
│   │   ├── Texture.h/.cpp     # RAII wrapper over SDL_Texture
│   │   ├── Animation.h/.cpp   # Frame data (one animation)
│   │   └── Animator.h/.cpp    # Playback controller (named animations)
│   └── main.cpp               # Entry point
├── tools/
│   └── generate_sprites.py    # Generates a test sprite sheet
├── vendored/
│   ├── SDL/                   # git submodule
│   └── SDL_image/             # git submodule
├── .github/workflows/         # CI configuration
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## 🏗 Architecture Overview

The engine follows a **layered, explicit** design:

- **`Application`** owns the window, renderer, and the main loop. It orchestrates `Input`, `Renderer`, and game state.
- **`Input`** stores both current and previous frame state, allowing three queries per key:
  - `IsKeyDown` — held this frame
  - `IsKeyPressed` — transitioned up → down this frame
  - `IsKeyReleased` — transitioned down → up this frame
- **`Renderer`** is a thin, non-owning wrapper over `SDL_Renderer*`. `Application` owns the underlying renderer.
- **`Texture`** is an RAII owner of `SDL_Texture*`. Copying is deleted; moving transfers ownership.
- **`Animation`** holds frame rectangles, frame time, and looping flag — pure data.
- **`Animator`** holds a map of named animations and drives the active one over time.

No hidden global state, no singletons. Everything is explicit and testable.

---

## 🗺 Roadmap

- [x] Window creation and game loop
- [x] Delta time calculation
- [x] Keyboard and mouse input
- [x] Texture loading and sprite rendering (SDL_image)
- [x] Sprite sheet animations (directional walk)
- [ ] Text rendering and FPS counter (SDL_ttf)
- [ ] Entity-Component-System
- [ ] Camera and tilemap
- [ ] Audio (SDL_mixer)
- [ ] Scene editor (ImGui)
- [ ] First demo game

---

## 🤝 Contributing

This is primarily a personal learning project, but **feedback, suggestions, and pull requests are welcome**.

If you find a bug or have an idea:

1. Open an issue describing the problem or proposal.
2. For code changes, fork the repository and open a pull request.
3. Follow the existing code style (`.clang-format` is provided).

There are no strict rules — clarity and correctness matter more than style.

---

## 📄 License

Released under the **MIT License** — see [LICENSE](LICENSE) for details.

You are free to use, modify, and distribute this code, including for commercial purposes.

---

## 🙏 Acknowledgements

- [SDL3](https://github.com/libsdl-org/SDL) — the multimedia layer this engine is built on.
- [SDL3_image](https://github.com/libsdl-org/SDL_image) — image loading.
- [Kenney.nl](https://kenney.nl/) — free, CC0-licensed game assets.