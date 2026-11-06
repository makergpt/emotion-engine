#pragma once
#include <string>
namespace EmotionEngine {
    namespace EUTL {
        enum class GraphicsBackend { OpenGL33, Vulkan13, DirectX12, Metal };
        struct SystemCapabilities { std::string gpuVendor; GraphicsBackend activeBackend; bool supportsSPIRV; };
        class TranslationLayer {
        public:
            static bool Initialize();
            static SystemCapabilities DetectEnvironment();
            static void BeginFrame();
            static void Shutdown();
        };
    }
}
