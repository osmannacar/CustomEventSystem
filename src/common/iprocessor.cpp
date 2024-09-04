#include "iprocessor.h"

IProcessor::IProcessor(EventDispatcher &dispatcher)
    : dispatcher(dispatcher)
    , running(false)
{

}

void IProcessor::start()
{
    running.store(true);
    workerThread = getThreadInfo();
}

void IProcessor::stop()
{
    running.store(false);
    queueCondition.notify_all(); // Wake up the thread if it's waiting
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void IProcessor::handleEvent(const Event &event)
{
    if(event.type == Event::Type::InitialState){
        return;
    }

    if (event.type == getAccessibleType()) {
        std::lock_guard<std::mutex> lock(queueMutex);
        frameQueue.push(event.data);
        queueCondition.notify_one();
    }
}
