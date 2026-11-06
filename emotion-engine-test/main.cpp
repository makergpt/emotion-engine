#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// --- EUTL / Graphics Math Subsystem ---
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
    static Mat4 rotationX(float angle) {
        Mat4 res = identity();
        float c = cosf(angle), s = sinf(angle);
        res.m[5] = c; res.m[6] = s;
        res.m[9] = -s; res.m[10] = c;
        return res;
    }
    static Mat4 rotationY(float angle) {
        Mat4 res = identity();
        float c = cosf(angle), s = sinf(angle);
        res.m[0] = c; res.m[2] = -s;
        res.m[8] = s; res.m[10] = c;
        return res;
    }
    static Mat4 translation(float x, float y, float z) {
        Mat4 res = identity();
        res.m[12] = x; res.m[13] = y; res.m[14] = z;
        return res;
    }
    static Mat4 scale(float x, float y, float z) {
        Mat4 res = identity();
        res.m[0] = x; res.m[5] = y; res.m[10] = z;
        return res;
    }
    Mat4 operator*(const Mat4& o) const {
        Mat4 res = {};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    res.m[i * 4 + j] += m[i * 4 + k] * o.m[k * 4 + j];
                }
            }
        }
        return res;
    }
};

// --- Professional Gray/Steel Shaders ---
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec3 aColor;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "out vec3 OurColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "   OurColor = aColor;\n"
    "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec3 OurColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   vec3 lightDir = normalize(vec3(1.0f, 3.0f, 2.0f));\n"
    "   vec3 norm = normalize(Normal);\n"
    "   float diff = max(dot(norm, lightDir), 0.3f);\n"
    "   vec3 result = diff * OurColor;\n"
    "   FragColor = vec4(result, 1.0f);\n"
    "}\0";

int main() {
    std::cout << "[INFO] Emotion Core: Initializing native application workstation...\n";
    
    if (!glfwInit()) {
        std::cerr << "[ERROR] Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Emotion Engine - Professional Workstation", NULL, NULL);
    if (!window) {
        std::cerr << "[ERROR] Failed to create workstation window.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "[ERROR] Failed to initialize GLEW.\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Neutral gray/slate metallic palette values
    float vertices[] = {
        // Front
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.55f, 0.58f, 0.62f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.55f, 0.58f, 0.62f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.55f, 0.58f, 0.62f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.55f, 0.58f, 0.62f,
        // Back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.35f, 0.38f, 0.42f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.35f, 0.38f, 0.42f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.35f, 0.38f, 0.42f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.35f, 0.38f, 0.42f,
        // Left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.45f, 0.48f, 0.52f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.45f, 0.48f, 0.52f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.45f, 0.48f, 0.52f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.45f, 0.48f, 0.52f,
        // Right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.48f, 0.51f, 0.55f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.48f, 0.51f, 0.55f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.48f, 0.51f, 0.55f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.48f, 0.51f, 0.55f,
        // Top
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.65f, 0.68f, 0.72f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.65f, 0.68f, 0.72f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.65f, 0.68f, 0.72f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.65f, 0.68f, 0.72f,
        // Bottom
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.27f, 0.30f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.27f, 0.30f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.27f, 0.30f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.27f, 0.30f
    };

    unsigned int indices[] = {
         0,  1,  2,   2,  3,  0,
         4,  5,  6,   6,  7,  4,
         8,  9, 10,  10, 11,  8,
        12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20
    };

    GLuint VBO, VAO, EBO;
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
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "[INFO] Workstation: Professional viewport active.\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Sober professional dark gray background (IDE/CAD style)
        glClearColor(0.16f, 0.18f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float time = (float)glfwGetTime();

        // Steady corporate camera framing
        Mat4 view = Mat4::lookAt(0.0f, 2.0f, 4.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
        Mat4 projection = Mat4::perspective(45.0f * (3.14159f / 180.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.m);

        glBindVertexArray(VAO);

        // Ground Platform Grid (Subdued slate)
        Mat4 floorModel = Mat4::translation(0.0f, -1.0f, 0.0f) * Mat4::scale(5.0f, 0.08f, 5.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, floorModel.m);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Center Workstation Object
        Mat4 coreModel = Mat4::translation(0.0f, 0.0f, 0.0f) * Mat4::rotationY(time * 0.4f) * Mat4::scale(0.75f, 0.75f, 0.75f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, coreModel.m);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
