#ifndef COMMANDLINEARGS_H
#define COMMANDLINEARGS_H

#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>

/*!
 * \brief Parses and manages command-line arguments for an application.
 * \details The CommandLineArgs class is designed to handle and validate command-line arguments
 * passed to an application. It parses arguments for model path, video path, and confidence
 * threshold, and provides methods to access these values. It also includes validation for paths
 * and thresholds to ensure they are in the correct format and are accessible.
 */

class CommandLineArgs {
public:
    /*!
     * \brief Constructs a CommandLineArgs object and parses command-line arguments.
     * \param argc The number of command-line arguments.
     * \param argv The array of command-line argument strings.
     * \details This constructor initializes the CommandLineArgs object and processes
     * the provided command-line arguments to extract and store relevant information.
     */
    CommandLineArgs(int argc, char* argv[]);

    /*!
     * \brief Gets the model path specified in the command-line arguments.
     * \return A string representing the path to the model.
     */
    std::string getModelPath() const;

    /*!
     * \brief Gets the video path specified in the command-line arguments.
     * \return A string representing the path to the video file.
     */
    std::string getVideoPath() const;

    /*!
     * \brief Gets the confidence threshold specified in the command-line arguments.
     * \return A double representing the confidence threshold.
     */
    double getConfidenceThreshold() const;

    /*!
     * \brief Validates the command-line arguments.
     * \return True if the arguments are valid; otherwise, false.
     * \details This method checks if the provided paths are valid and if the arguments meet
     * the expected format or criteria. It ensures the arguments are suitable for further processing.
     */
    bool validateArguments() const;

    /*!
     * \brief Prints the usage message for the command-line application.
     * \param programName The name of the program to include in the usage message.
     * \details This static method outputs a message detailing how to use the command-line application,
     * including the available arguments and their expected formats.
     */
    static void printUsage(const char* programName);

private:
    /*!
     * \brief Parses command-line arguments.
     * \param argc The number of command-line arguments.
     * \param argv The array of command-line argument strings.
     * \details This method processes the command-line arguments to extract and store
     * relevant values such as model path, video path, and confidence threshold.
     */
    void parseArguments(int argc, char* argv[]);

    /*!
     * \brief Validates if a given path is a valid URL or file.
     * \param path The path to be validated.
     * \return True if the path is valid; otherwise, false.
     * \details This method checks whether the provided path is either a valid URL or
     * a file path that meets specific criteria.
     */
    bool validatePath(const std::string& path) const;

    /*!
     * \brief Checks if a given string is a valid URL.
     * \param url The URL string to be checked.
     * \return True if the URL is valid; otherwise, false.
     */
    static bool isValidUrl(const std::string& url);

    /*!
     * \brief Checks if a file exists at the given path.
     * \param path The file path to be checked.
     * \return True if the file exists; otherwise, false.
     */
    static bool fileExists(const std::string& path);

private:
    /*!
    * \brief Path to the model file.
    * \details Stores the file path of the model that will be used for inference or processing tasks.
    */
    std::string modelPath;

    /*!
    * \brief Path to the video file.
    * \details Stores the file path of the video from which frames will be extracted and processed.
    */
    std::string videoPath;

    /*!
    * \brief Confidence threshold for model predictions.
    * \details Specifies the minimum confidence score required for a prediction to be considered valid. The default value is 0.3.
    */
    double confidenceThreshold = 0.3;
};

#endif // COMMANDLINEARGS_H
