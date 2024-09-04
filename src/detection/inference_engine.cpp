#include "inference_engine.h"
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

InferenceEngine::InferenceEngine(const std::string& cfgPath, const std::string& weightsPath,
                                 const std::string& classesPath, const std::string& colorsPath, float confidenceThreshold, EventDispatcher& dispatcher)
    : IProcessor(dispatcher)
    , cfgPath(cfgPath)
    , weightsPath(weightsPath)
    , classesPath(classesPath)
    , colorsPath(colorsPath)
    , confidenceThreshold(confidenceThreshold)
{

    parseClassName(classesPath.c_str());
    parseRgbColors(colorsPath.c_str());

}

InferenceEngine::~InferenceEngine() {
    stop();
}


void InferenceEngine::processEvents() {
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(cfgPath, weightsPath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    while (running.load()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [this]() { return !frameQueue.empty() || !running.load(); });

        if (!running.load()) break; // Exit if not running

        std::pair<cv::Mat, cv::Mat> frames = frameQueue.front();
        frameQueue.pop();
        lock.unlock();

        // Perform inference
        cv::Mat blob;
        cv::dnn::blobFromImage(frames.second, blob, 1.0 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
        net.setInput(blob);
        std::vector<cv::Mat> detections;
        net.forward(detections, net.getUnconnectedOutLayersNames());

        // Process detections
        std::vector<cv::Rect> boxes;
        for (auto& detection : detections) {
            for (int i = 0; i < detection.rows; ++i) {
                const int probability_index = 5;
                const int probability_size = detection.cols - probability_index;
                float* prob_array_ptr = &detection.at<float>(i, probability_index);
                size_t objectClass = std::max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
                float confidence = detection.at<float>(i, (int)objectClass + probability_index);

                if (confidence > confidenceThreshold) {
                    float x_center = detection.at<float>(i, 0) * frames.second.cols;
                    float y_center = detection.at<float>(i, 1) * frames.second.rows;
                    float width = detection.at<float>(i, 2) * frames.second.cols;
                    float height = detection.at<float>(i, 3) * frames.second.rows;
                    cv::Rect box((int)(x_center - width / 2), (int)(y_center - height / 2), (int)width, (int)height);
                    boxes.push_back(box);
                    cv::rectangle(frames.second, box, colors[objectClass], 2);

                    // Add class name text
                    std::string label = classes[objectClass];
                    int baseLine;
                    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                    int top = std::max(box.y, labelSize.height);
                    cv::putText(frames.second, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, colors[objectClass], 2);
                }
            }
        }

        // Process detections and post event
        dispatcher.postEvent(Event(Event::Type::FrameDetectionReady, frames));
        frames.second.release();
    }
}

Event::Type InferenceEngine::getAccessibleType()
{
    return Event::Type::FrameDefoggerReady;
}

std::thread InferenceEngine::getThreadInfo()
{
    return std::thread(&InferenceEngine::processEvents, this);
}

void InferenceEngine::parseRgbColors(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        int r, g, b;

        if (std::getline(iss, token, ',') && std::istringstream(token) >> r &&
            std::getline(iss, token, ',') && std::istringstream(token) >> g &&
            std::getline(iss, token) && std::istringstream(token) >> b) {
            colors.push_back(cv::Scalar(b, g, r));
        }
    }

    file.close();
}

void InferenceEngine::parseClassName(const std::string &filePath)
{
    std::ifstream file(classesPath.c_str());
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        classes.push_back(line);
    }
}
