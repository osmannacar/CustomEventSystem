#include "gui_renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

GUIRenderer::GUIRenderer(EventDispatcher& dispatcher)
    : IProcessor(dispatcher) {}

GUIRenderer::~GUIRenderer() {
    stop();
}

void GUIRenderer::processEvents() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Object Detection", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    GLuint texture1 = 0; // Texture ID for frames.first
    GLuint texture2 = 0; // Texture ID for frames.second

    while (running.load() && !glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Object Detection");

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [this]() { return !frameQueue.empty() || !running.load(); });

            if (!running.load()) break; // Exit if not running

            std::pair<cv::Mat, cv::Mat> frames = frameQueue.front();
            frameQueue.pop();
            lock.unlock();

            // Render frames.first
            renderFrame(frames.first, texture1, "Unsupported image format for orjinal video frame");
            frames.first.release();

            // Render frames.second
            renderFrame(frames.second, texture2, "Unsupported image format for processed video frame");
            frames.second.release();
        }

        ImGui::End();
        ImGui::Render();

        // Render ImGui data
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Shutdown and cleanup
    dispatcher.shutdownEventloop();

    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

Event::Type GUIRenderer::getAccessibleType()
{
    return Event::Type::FrameDetectionReady;
}

std::thread GUIRenderer::getThreadInfo()
{
    return std::thread(&GUIRenderer::processEvents, this);
}

void GUIRenderer::renderFrame(const cv::Mat &frame, GLuint &texture, const std::string &errorMessage) {
    if (!frame.empty()) {
        cv::Mat imgRGBA;
        if (frame.channels() == 3) {
            cv::cvtColor(frame, imgRGBA, cv::COLOR_BGR2RGBA);
        } else if (frame.channels() == 4) {
            imgRGBA = frame;
        } else {
            std::cerr << errorMessage << std::endl;
            imgRGBA.release();
            return;
        }

        if (texture == 0) {
            glGenTextures(1, &texture);
        }
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGBA.cols, imgRGBA.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgRGBA.data);

        ImGui::SameLine(); // Ensure the next widget is on the same line
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), ImVec2(imgRGBA.cols, imgRGBA.rows));
    }
}
