void EditorClientBlackBerry::handleKeyboardEvent(KeyboardEvent* event)
{
    ASSERT(event);

    const PlatformKeyboardEvent* platformEvent = event->keyEvent();
    if (!platformEvent)
        return;

    ASSERT(event->target()->toNode());
    Frame* frame = event->target()->toNode()->document()->frame();
    ASSERT(frame);

    String commandName = interpretKeyEvent(event);

    ASSERT(!(event->type() == eventNames().keydownEvent && frame->editor()->command(commandName).isTextInsertion()));

    if (!commandName.isEmpty()) {
        if (commandName != "DeleteBackward")
            m_webPagePrivate->m_inputHandler->setProcessingChange(false);

        if (frame->editor()->command(commandName).execute())
            event->setDefaultHandled();
        return;
    }

    if (!frame->editor()->canEdit())
        return;

    if (event->type() != eventNames().keypressEvent)
        return;

    if (event->charCode() < ' ')
        return;

    if (event->ctrlKey() || event->altKey())
        return;

    if (!platformEvent->text().isEmpty()) {
        if (frame->editor()->insertText(platformEvent->text(), event))
            event->setDefaultHandled();
    }
}
