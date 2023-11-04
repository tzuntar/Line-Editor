#ifndef LINEEDIT_MESSAGE_H
#define LINEEDIT_MESSAGE_H

#include <vector>

namespace Editor {

    /* Here's how the message-driven architecture of the Line Editor UI works:
     *
     *  ._________.     ._________.   .____________.
     *  | Emitter |--->/ Message /--->| Dispatcher |
     *  '''''''''''    ''''''''''     ''''''''''''''
     *                              ._______/
     *                              |
     *  Message:              ._____v____.
     *   - Type               | Receiver |
     *   - Context (data*)    ''''''''''''
     */

    /// Represents possible types of a Message.
    enum MessageType {
        /// Any kind of message / undefined. Expect it to be received by
        /// various (possibly conflicting) handlers.
        GLOBAL,

        /// Keyboard commands, 'context' must point to 'has_key(int ch)'-compatible
        /// info about the pressed key.
        KEYBOARD_COMMAND,

        /// Context-switching message. 'context' must point to an int specifying
        /// the new context.
        SWITCH_CONTEXT,

        // Messages to be forwarded to the text editor for further processing.

        /// Load the file from the handle in 'context'.
        EDITOR_LOAD_FILE,
        /// Clear and initialize a blank file.
        EDITOR_CLEAR_FILE,
        /// Write to the currently loaded file.
        EDITOR_WRITE_FILE
    };

    /// An internal message to another part of the UI.
    struct Message {
        /// Type of the message.
        MessageType type = GLOBAL;

        /// Context data. See MessageType for more info.
        void *content = nullptr;
    };

    /// Receives messages from the global dispatcher.
    class MessageHandler {
    protected:
        /// Specifies which messages the class should be receiving
        /// from the dispatcher.
        MessageType receivedMessageType = GLOBAL;
    public:
        /// Called by the dispatcher.
        virtual void receiveMessage(Message message /// <[in] Received message.
        ) = 0;

        MessageType getReceivedMessageType() { return receivedMessageType; }
    };

    /// A router for sending messages across components.
    class MessageDispatcher {
    protected:
        /// Prevent multiple instances
        MessageDispatcher() = default;

        static MessageDispatcher *dispatcher;

    private:
        std::vector<MessageHandler *> handlers;

    public:
        /// Dispatch a message to all registered handlers
        void dispatchMessage(Message message    /// <[in] Message to dispatch.
        );

        /// Register this handler.
        /// Returns 'true' on successful registration.
        bool registerHandler(MessageHandler *handler    /// <[in] Pointer to the handler.
        );

        static MessageDispatcher *GetInstance();

        // Prevent various mischiefs

        MessageDispatcher(MessageDispatcher *other) = delete;
        void operator=(const MessageDispatcher &) = delete;


    };

}  // Editor

#endif //LINEEDIT_MESSAGE_H
