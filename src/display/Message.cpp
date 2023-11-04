#include "Message.h"

namespace Editor {

    MessageDispatcher *MessageDispatcher::GetInstance() {
        if (dispatcher == nullptr)
            dispatcher = new MessageDispatcher();
        return dispatcher;
    }

    void MessageDispatcher::dispatchMessage(Editor::Message message) {
        for (const auto handler: handlers) {
            if (handler->getReceivedMessageType() == message.type) {
                handler->receiveMessage(message);
            }
        }
    }

    bool MessageDispatcher::registerHandler(Editor::MessageHandler *handler) {
        if (std::find(handlers.begin(), handlers.end(), handler) != handlers.end())
            return false;
        handlers.emplace_back(handler);
        return true;
    }

}  // Editor
