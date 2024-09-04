#include <iostream>
#include <string>
#include "video_processor.h"
#include "inference_engine.h"
#include "gui_renderer.h"
#include "defogger.h"
#include "commandline_args.h"

int main(int argc, char** argv) {

    // Parse command-line arguments
    CommandLineArgs cmdArgs(argc, argv);

    // Validate command-line arguments and print usage if invalid
    if (!cmdArgs.validateArguments()) {
        CommandLineArgs::printUsage(argv[0]);
        return 1;
    }

    // Create an EventDispatcher to manage event handling
    EventDispatcher dispatcher;

    // Initialize the VideoProcessor with the path to the video file and the dispatcher
    VideoProcessor videoProcessor(cmdArgs.getVideoPath(), dispatcher);

    // Initialize the Defogger with the dispatcher
    Defogger defogger(dispatcher);

    // Initialize the InferenceEngine with paths to model configuration, weights, class names, colors, and the confidence threshold
    InferenceEngine inferenceEngine(
        cmdArgs.getModelPath() + "/yolov3.cfg",
        cmdArgs.getModelPath() + "/yolov3.weights",
        cmdArgs.getModelPath() + "/coco_classes.txt",
        cmdArgs.getModelPath() + "/coco_colors.txt",
        cmdArgs.getConfidenceThreshold(),
        dispatcher
        );

    // Initialize the GUIRenderer with the dispatcher
    GUIRenderer guiRenderer(dispatcher);

    // Register event handlers for various event types
    dispatcher.registerHandler(
        Event::Type::FrameCaptureReady,
        std::bind(&Defogger::handleEvent, &defogger, std::placeholders::_1)
        );
    dispatcher.registerHandler(
        Event::Type::FrameDefoggerReady,
        std::bind(&InferenceEngine::handleEvent, &inferenceEngine, std::placeholders::_1)
        );
    dispatcher.registerHandler(
        Event::Type::FrameDetectionReady,
        std::bind(&GUIRenderer::handleEvent, &guiRenderer, std::placeholders::_1)
        );

    // Start processing in all components
    guiRenderer.start();
    inferenceEngine.start();
    defogger.start();
    videoProcessor.start();

    // Start the event loop to process events
    dispatcher.startEventloop();

    // Stop all components after the event loop ends
    videoProcessor.stop();
    defogger.stop();
    inferenceEngine.stop();
    guiRenderer.stop();

    return 0;
}
