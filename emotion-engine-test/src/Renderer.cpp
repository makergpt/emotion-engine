#include "EmotionEngine/Graphics/Renderer.hpp"
#include <iostream>
#include <cmath>

struct Mat4 {
    float m[16] = {0};
    static Mat4 identity() {
        Mat4 res;
        res.m[0] = res.m[5] = res.m[10] = res.m[15] = 1.0f;
        return res;
    }
    static Mat4 perspective(float fov, float aspect, float near, float far) {
        Mat4 res = {};
        float tanHalfFov = tanf(fov / 2.0f);
        res.m[0] = 1.0f / (aspect * tanHalfFov);
        res.m[5] = 1.0f / tanHalfFov;
        res.m[10] = -(far + near) / (far - near);
        res.m[11] = -1.0f;
        res.m[14] = -(2.0f * far * near) / (far - near);
        return res;
    }
    static Mat4 lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
        float fX = centerX - eyeX, fY = centerY - eyeY, fZ = centerZ - eyeZ;
        float rLen = 1.0f / sqrtf(fX*fX + fY*fY + fZ*fZ);
        fX *= rLen; fY *= rLen; fZ *= rLen;

        float sX = fY * upZ - fZ * upY, sY = fZ * upX - fX * upZ, sZ = fX * upY - fY * upX;
        rLen = 1.0f / sqrtf(sX*sX + sY*sY + sZ*sZ);
        sX *= rLen; sY *= rLen; sZ *= rLen;

        float uX = sY * fZ - sZ * fY, uY = sZ * fX - sX * fZ, uZ = sX * fY - sY * fX;

        Mat4 res = identity();
        res.m[0] = sX; res.m[4] = sY; res.m[8]  = sZ;
        res.m[1] = uX; res.m[5] = uY; res.m[9]  = uZ;
        res.m[2] =-fX; res.m[6] =-fY; res.m[10] =-fZ;
        res.m[12] = -(sX * eyeX + sY * eyeY + sZ * eyeZ);
        res.m[13] = -(uX * eyeX + uY * eyeY + uZ * eyeZ);
        res.m[14] =  (fX * eyeX + fY * eyeY + fZ * eyeZ);
        return res;
    }
    static Mat4 rotationY(float angle) {
        Mat4 res = identity();
        float c = cosf(angle), s = sinf(angle);
        res.m[0] = c; res.m[2] = -s;
        res.m[8] = s; res.m[10] = c;
        return res;
    }
};

const char* vertexShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec3 aColor;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "out vec3 OurColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "   OurColor = aColor;\n"
    "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\0";

const char* fragmentShaderSrc = "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec3 OurColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   vec3 lightDir = normalize(vec3(1.0f, 3.0f, 2.0f));\n"
    "   float diff = max(dot(normalize(Normal), lightDir), 0.3f);\n"
    "   FragColor = vec4(diff * OurColor, 1.0f);\n"
    "}\0";

namespace EmotionEngine {
    namespace Graphics {
        GLFWwindow* window = nullptr;
        GLuint shaderProgram;
        GLuint VAO, VBO, EBO;

        bool Renderer::Initialize(int width, int height, const char* title) {
            if (!glfwInit()) return false;
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            if (!window) { glfwTerminate(); return false; }
            glfwMakeContextCurrent(window);

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) return false;
            glEnable(GL_DEPTH_TEST);

            // Compile shaders
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertexShaderSrc, NULL);
            glCompileShader(vs);

            GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
            glCompileShader(fs);

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vs);
            glAttachShader(shaderProgram, fs);
            glLinkProgram(shaderProgram);
            glDeleteShader(vs);
            glDeleteShader(fs);

            // Cube Vertices (Position, Normal, Color)
            float vertices[] = {
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.6f, 0.6f, 0.65f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.6f, 0.6f, 0.65f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.6f, 0.6f, 0.65f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.6f, 0.6f, 0.65f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.4f, 0.4f, 0.45f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.4f, 0.4f, 0.45f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.4f, 0.4f, 0.45f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.4f, 0.4f, 0.45f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.5f, 0.5f, 0.55f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.5f, 0.5f, 0.55f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.5f, 0.5f, 0.55f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.5f, 0.5f, 0.55f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.55f, 0.55f, 0.6f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.55f, 0.55f, 0.6f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.55f, 0.55f, 0.6f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.55f, 0.55f, 0.6f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.7f, 0.7f, 0.75f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.7f, 0.7f, 0.75f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.7f, 0.7f, 0.75f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.7f, 0.7f, 0.75f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.3f, 0.3f, 0.35f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.3f, 0.3f, 0.35f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.3f, 0.3f, 0.35f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.3f, 0.3f, 0.35f
            };

            unsigned int indices[] = {
                 0,  1,  2,   2,  3,  0,
                 4,  5,  6,   6,  7,  4,
                 8,  9, 10,  10, 11,  8,
                12, 13, 14,  14, 15, 12,
                16, 17, 18,  18, 19, 16,
                20, 21, 22,  22, 23, 20
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
            return true;
        }

        bool Renderer::ShouldClose() { return glfwWindowShouldClose(window); }
        void Renderer::PollEvents() { glfwPollEvents(); }

        void Renderer::RenderFrame() {
            glClearColor(0.14f, 0.16f, 0.20f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            float time = (float)glfwGetTime();
            Mat4 model = Mat4::rotationY(time * 0.75f);
            Mat4 view = Mat4::lookAt(0.0f, 2.0f, 4.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
            Mat4 projection = Mat4::perspective(45.0f * (3.14159f / 180.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model.m);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view.m);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.m);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
        }

        void Renderer::Shutdown() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteProgram(shaderProgram);
            if (window) glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
}
