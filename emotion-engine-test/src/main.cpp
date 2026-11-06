#include "EmotionEngine/EUTL/TranslationLayer.hpp"
#include "EmotionEngine/EUTL/AppInterceptor.hpp"
#include "EmotionEngine/Graphics/Renderer.hpp"
#include <iostream>
int main(int argc, char* argv[]) {
    using namespace EmotionEngine;
    std::string targetApp = (argc > 1) ? argv[1] : "DefaultWorkstation";
    if (!EUTL::TranslationLayer::Initialize()) return -1;
    EUTL::AppInterceptor::AttachExternalApp(targetApp, nullptr);
    Graphics::Renderer renderer;
    if (!renderer.Initialize(1280, 720, ("Emotion Engine - Interceptor Viewport: " + targetApp).c_str())) {
        std::cerr << "[ERROR] Initialization failed.\n";
        return -1;
    }
    while (!renderer.ShouldClose()) {
        renderer.PollEvents();
        EUTL::AppInterceptor::TranslateAndExecuteFrame();
        renderer.RenderFrame();
    }
    EUTL::AppInterceptor::DetachApp();
    renderer.Shutdown();
    EUTL::TranslationLayer::Shutdown();
    return 0;
}
