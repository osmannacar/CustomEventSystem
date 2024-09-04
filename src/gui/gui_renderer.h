#ifndef GUIRENDERER_H
#define GUIRENDERER_H

#include <string>
#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include "iprocessor.h"

/*!
 * \brief Responsible for rendering graphical user interface (GUI) elements.
 * \details The GUIRenderer class inherits from the IProcessor interface and provides
 * implementation for processing events, accessing event types, and managing threading
 * for rendering operations. This class is responsible for handling and displaying GUI
 * elements related to the application's functionality.
 */
class GUIRenderer : public IProcessor {
public:
    /*!
     * \brief Constructs a GUIRenderer object.
     * \param dispatcher Reference to the EventDispatcher used for posting and handling events.
     * \details Initializes the GUIRenderer with an EventDispatcher instance to manage events
     * and handle GUI rendering tasks.
     */
    GUIRenderer(EventDispatcher& dispatcher);

    /*!
     * \brief Destructs the GUIRenderer object.
     * \details Cleans up resources used by the GUIRenderer and ensures a proper shutdown.
     */
    ~GUIRenderer();

private:
    /*!
     * \brief Processes events specific to GUI rendering.
     * \details This method overrides the processEvents function from the IProcessor interface.
     * It handles events related to GUI rendering, such as updating or drawing GUI elements.
     */
    void processEvents() override;

    /*!
     * \brief Retrieves the type of events that the GUIRenderer can handle.
     * \return The type of events that the GUIRenderer is capable of processing.
     * \details This method overrides the getAccessibleType function from the IProcessor interface.
     * It returns the specific event type that the GUIRenderer is designed to handle.
     */
    Event::Type getAccessibleType() override;

    /*!
     * \brief Retrieves the thread associated with GUI rendering operations.
     * \return A std::thread object representing the thread used for GUI rendering tasks.
     * \details This method overrides the getThreadInfo function from the IProcessor interface.
     * It provides access to the thread that executes GUI rendering operations.
     */
    std::thread getThreadInfo() override;

private:
    /*!
    * \brief Renders a given frame to an OpenGL texture and displays it using ImGui.
    * \param frame The input image frame to be rendered.
    * \param texture Reference to the texture ID to be used for rendering. This ID is generated if it is not already created.
    * \param errorMessage The message to be displayed if the image format of the frame is unsupported.
    *
    * \details This function processes the input frame to convert it into an RGBA format suitable for OpenGL texture upload.
    * It checks the number of channels in the input frame and performs the necessary conversion. If the frame is valid, it
    * creates or updates an OpenGL texture with the frame data, sets the appropriate texture parameters, and uses ImGui to
    * display the image. If the frame has an unsupported image format, an error message is printed to the standard error stream.
    */
    void renderFrame(const cv::Mat& frame, GLuint& texture, const std::string& errorMessage);
};

#endif // GUIRENDERER_H
