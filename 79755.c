void HTMLTextAreaElement::defaultEventHandler(Event* event)
{
    if (renderer() && (event->isMouseEvent() || event->isDragEvent() || event->hasInterface(EventNames::WheelEvent) || event->type() == EventTypeNames::blur))
        forwardEvent(event);
    else if (renderer() && event->isBeforeTextInsertedEvent())
        handleBeforeTextInsertedEvent(static_cast<BeforeTextInsertedEvent*>(event));

    HTMLTextFormControlElement::defaultEventHandler(event);
}
