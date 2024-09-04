# CustomEventSystem

Here’s an updated README that includes information about the custom event system (`EventDispatcher`) used in your C++ application. This version integrates details about how the custom event system is used for handling events in the video processing pipeline.

---

# C++ Video Processing with YOLO V3

## Overview

This C++ application performs video processing using the YOLO V3 object detection model. The application reads a video file, processes it frame-by-frame using a pre-trained YOLO V3 model, and displays the results in a GUI window using Dear ImGui. The application uses a custom event system to manage the flow of data between different components.

## Features

1. **Command Line Input:** Accepts a video file path from the command line.
2. **Video Processing:** Loads and reads video frames in a separate thread.
3. **Inference:** Uses OpenCV DNN module to run YOLO V3 object detection.
4. **Visualization:** Draws bounding boxes around detected objects using class-specific colors.
5. **GUI Display:** Displays the original and processed video frames in a window using Dear ImGui.
6. **Custom Event System:** Manages event dispatching and handling using a custom `EventDispatcher` class.


### Dependencies

1. **OpenCV:** Install OpenCV with DNN module support. Follow the [OpenCV installation guide](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) if needed.
2. **Dear ImGui:** Clone and integrate Dear ImGui into your project. You can find it at [Dear ImGui GitHub repository](https://github.com/ocornut/imgui).


## Code Structure

- `main.cpp`: The entry point of the application. Handles command line input, video processing, and GUI display.
- `video_processor.cpp` / `video_processor.h`: Contains the logic for video frame reading and threading.
- `yolo_detector.cpp` / `yolo_detector.h`: Implements YOLO V3 inference and post-processing.
- `gui_display.cpp` / `gui_display.h`: Manages the Dear ImGui GUI for displaying frames.
- `event_dispatcher.cpp` / `event_dispatcher.h`: Implements the custom event system.

## Custom Event System

### Event Class

The `Event` class represents an event with a type and associated data. It encapsulates information about events, including the type of event and data such as original and processed images. The `Event` class is defined as follows:

```cpp
class Event {
public:
    enum class Type {
        InitialState,
        FrameCaptureReady,
        FrameDefoggerReady,
        FrameDetectionReady
    };

    Event(Type type, std::pair<cv::Mat, cv::Mat> data);
    
    Type type;
    std::pair<cv::Mat, cv::Mat> data;
};
```

### EventDispatcher Class

The `EventDispatcher` class manages event dispatching and handling. It maintains a queue of events, handles thread synchronization, and dispatches events to appropriate handlers. Key methods include:

- **`postEvent(const Event& event)`**: Adds an event to the queue for processing.
- **`registerHandler(Event::Type type, std::function<void(const Event&)> handler)`**: Registers a handler function for a specific event type.
- **`startEventloop()`**: Starts the event loop to process and dispatch events.
- **`shutdownEventloop()`**: Stops the event loop and performs cleanup.

The `EventDispatcher` class is defined as follows:

```cpp
class EventDispatcher {
public:
    EventDispatcher();
    ~EventDispatcher();

    void postEvent(const Event& event);
    void registerHandler(Event::Type type, std::function<void(const Event&)> handler);
    void startEventloop();
    void shutdownEventloop();

private:
    std::atomic<bool> running;
    std::queue<Event> eventQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    std::map<Event::Type, std::function<void(const Event&)>> handlerContainer;
};
```
---

This README now includes detailed information about the custom event system used in your application, helping users understand how the event management works alongside video processing and object detection.
