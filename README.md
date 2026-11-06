# 🛡️ Emotion Engine

**Universal Cross-Platform Software, Graphics & Application Interception Engine**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](LICENSE)
[![C++17](https://img.shields.io/badge/C++-17-orange.svg?style=flat-square)]()
[![OpenGL 3.3](https://img.shields.io/badge/Backend-OpenGL%203.3%20%2F%20Vulkan-green.svg?style=flat-square)]()

*One engine. Every platform. No unnecessary limitations.*

</div>

---

## 🌟 Overview

**Emotion Engine** is a universal software and graphics technology platform designed to eliminate the fragmentation of writing platform-specific code. By abstracting window management, hardware detection, and graphics pipelines behind the **Emotion Universal Translation Layer (EUTL)**, developers can write once and run anywhere.

---

## 🏗️ Architecture & Subsystems

The engine is structured as an independent, modular technology platform divided into specialized subsystems:

| Subsystem | Responsibility |
| :--- | :--- |
| **Emotion Core** | Manages core infrastructure, memory allocators, logging, and thread pools. |
| **Emotion Graphics** | Provides unified rendering interfaces, pipeline state objects (PSOs), and swapchain abstractions. |
| **Emotion EUTL** | Automatically detects hardware capabilities and translates engine instructions into native platform operations. |
| **Emotion Shader** | Handles cross-compilation, validation, and optimization of shaders across SPIR-V, HLSL, GLSL, MSL, and WGSL. |
| **Emotion Build** | Manages dependency graphing, caching, toolchain selection, and cross-platform packaging. |
| **App Interceptor** | Hooks foreign binaries and external application windows, routing them through offscreen framebuffers (FBO). |

---

## 🚀 Master CLI Command Reference (`emotion`)

Once installed globally, the `emotion` CLI provides a unified command set for managing your development workstation and runtime:

```bash
emotion start             # Launch core workstation viewport
emotion launch <app>      # Intercept and run external app (e.g. sober)
emotion test              # Run engine diagnostic test suite
emotion shutdown          # Cleanly terminate active engine sessions
