#ifndef INFERENCEENGINE_H
#define INFERENCEENGINE_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "iprocessor.h"

/*!
 * \brief Handles inference tasks for object detection and classification.
 * \details The InferenceEngine class is responsible for running inference algorithms
 * using a pre-trained model to detect and classify objects in images or video frames.
 * It integrates with the IProcessor interface to manage the processing of frames and
 * handle events. This class handles model configuration, weight loading, and class
 * names parsing, as well as maintaining a list of RGB colors for visualizing detections.
 */
class InferenceEngine : public IProcessor {
public:
    /*!
     * \brief Constructs an InferenceEngine object with specified model and configuration paths.
     * \param cfgPath Path to the configuration file for the model.
     * \param weightsPath Path to the weights file for the model.
     * \param classesPath Path to the file containing class names.
     * \param colorsPath Path to the file containing RGB colors for visualizations.
     * \param confidenceThreshold Minimum confidence threshold for detecting objects.
     * \param dispatcher Reference to the EventDispatcher for event handling.
     * \details Initializes the InferenceEngine with the given paths and settings.
     * It prepares the necessary resources and configurations required for running
     * inference tasks on images or video frames.
     */
    InferenceEngine(const std::string& cfgPath, const std::string& weightsPath,
                    const std::string& classesPath, const std::string& colorsPath, float confidenceThreshold, EventDispatcher& dispatcher);

    /*!
     * \brief Destroys the InferenceEngine object and releases resources.
     * \details Cleans up any resources used by the InferenceEngine, including
     * releasing memory and shutting down any ongoing processes.
     */
    ~InferenceEngine();

protected:
    /*!
     * \brief Processes events related to inference tasks.
     * \details This overridden method is responsible for handling and processing
     * events related to object detection and classification. The specific logic
     * will depend on the type of events and the needs of the application.
     */
    void processEvents() override;

    /*!
     * \brief Retrieves the type of events that the InferenceEngine can handle.
     * \return Event::Type The type of events that this class is capable of handling.
     * \details This overridden method provides information about the type of events
     * that are relevant to the InferenceEngine, such as those related to frame processing
     * or object detection.
     */
    Event::Type getAccessibleType() override;

    /*!
     * \brief Retrieves the thread associated with the InferenceEngine.
     * \return std::thread The thread used for executing inference tasks.
     * \details This overridden method returns the thread object used for performing
     * inference operations, which can be moved or managed as needed.
     */
    std::thread getThreadInfo() override;

private:
    /*!
     * \brief Parses RGB colors from a specified file.
     * \param filePath Path to the file containing RGB color definitions.
     * \details Reads and parses RGB colors from the given file to be used for
     * visualizing detection results. The colors are stored in a vector for use
     * during rendering.
     */
    void parseRgbColors(const std::string& filePath);

    /*!
     * \brief Parses class names from a specified file.
     * \param filePath Path to the file containing class names.
     * \details Reads and parses class names from the given file, which will be
     * used for identifying and labeling detected objects in images or video frames.
     */
    void parseClassName(const std::string& filePath);

private:
    /*!
    * \brief Path to the model configuration file.
    * \details This string holds the file path to the configuration file used to set up the model's parameters and structure.
    */
    std::string cfgPath;

    /*!
    * \brief Path to the model weights file.
    * \details This string holds the file path to the weights file that contains the pre-trained model parameters.
    */
    std::string weightsPath;

    /*!
    * \brief Path to the file containing class names.
    * \details This string holds the file path to a file that lists the names of the classes the model can detect.
    */
    std::string classesPath;

    /*!
    * \brief Path to the file containing RGB colors.
    * \details This string holds the file path to a file that specifies the RGB colors used for visualizing different classes.
    */
    std::string colorsPath;

    /*!
    * \brief Minimum confidence threshold for detections.
    * \details This float value represents the minimum confidence level required for a detection to be considered valid. Detections with a confidence score below this threshold will be ignored.
    */
    float confidenceThreshold;

    /*!
    * \brief List of class names for object detection.
    * \details This vector holds the names of the classes that the model is trained to detect. It is populated from the file specified by `classesPath`.
    */
    std::vector<std::string> classes;

    /*!
    * \brief List of RGB colors for visualization.
    * \details This vector holds RGB color values used to visualize the different classes in detection results. Colors are assigned to classes for easy differentiation.
    */
    std::vector<cv::Scalar> colors;
};

#endif // INFERENCEENGINE_H
