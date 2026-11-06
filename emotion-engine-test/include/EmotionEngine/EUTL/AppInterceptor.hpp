#pragma once
#include <string>
namespace EmotionEngine {
    namespace EUTL {
        class AppInterceptor {
        public:
            static bool AttachExternalApp(const std::string& appName, void* nativeWindowHandle);
            static void TranslateAndExecuteFrame();
            static void DetachApp();
        };
    }
}
