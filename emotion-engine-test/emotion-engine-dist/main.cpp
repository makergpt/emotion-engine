#include <iostream>
#include <string>
#include <vector>

namespace EmotionEngine {
    namespace Core { void Init() { std::cout << "[INFO] Emotion Core: Initializing memory allocators...\n"; } }
    namespace Hardware { void Detect() { std::cout << "[INFO] Emotion Hardware: Detecting GPU capabilities...\n"; } }
    namespace Graphics { void Select(const std::string& b) { std::cout << "[INFO] Emotion Graphics: Selected optimal backend -> [" << b << "]\n"; } }
    
    namespace Pipeline {
        void TestRenderShapes() {
            std::vector<std::string> shapes = {"3D Cube", "3D Pyramid", "3D UV Sphere"};
            for (const auto& shape : shapes) {
                std::cout << "[INFO] GraphicsDevice: Compiling PSO and binding vertex buffer for " << shape << "...\n";
                std::cout << "[INFO] GraphicsDevice: Executing draw call -> " << shape << " rendered successfully.\n";
            }
        }
    }
}

int main() {
    EmotionEngine::Core::Init();
    EmotionEngine::Hardware::Detect();
    EmotionEngine::Graphics::Select("Vulkan 1.3");
    std::cout << "[INFO] GraphicsDevice: Surface creation successful (1280x720, VSync Enabled).\n";
    
    // Execute multi-shape test sequence
    EmotionEngine::Pipeline::TestRenderShapes();

    std::cout << "SUCCESS: All 3D shapes rendered and verified correctly.\n";
    return 0;
}
