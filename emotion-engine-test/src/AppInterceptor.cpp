#include "EmotionEngine/EUTL/AppInterceptor.hpp"
#include <GL/glew.h>
#include <iostream>
namespace EmotionEngine {
    namespace EUTL {
        static std::string activeTargetApp = "";
        bool AppInterceptor::AttachExternalApp(const std::string& appName, void* nativeWindowHandle) {
            activeTargetApp = appName;
            std::cout << "[INFO] EUTL Interceptor: Hooked target application -> [" << activeTargetApp << "]\n";
            return true;
        }
        void AppInterceptor::TranslateAndExecuteFrame() {
            glClearColor(0.12f, 0.14f, 0.18f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        void AppInterceptor::DetachApp() {
            std::cout << "[INFO] EUTL Interceptor: Detached application session [" << activeTargetApp << "].\n";
        }
    }
}
