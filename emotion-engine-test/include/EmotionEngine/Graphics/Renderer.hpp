#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace EmotionEngine {
    namespace Graphics {
        class Renderer {
        private:
            GLFWwindow* window;
        public:
            bool Initialize(int width, int height, const char* title);
            void RenderFrame();
            void Shutdown();
            bool ShouldClose();
            void PollEvents();
        };
    }
}
