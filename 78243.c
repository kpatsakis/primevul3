void HTMLFormElement::handleLocalEvents(Event* event)
{
    Node* targetNode = event->target()->toNode();
    if (event->eventPhase() != Event::CAPTURING_PHASE && targetNode && targetNode != this && (event->type() == eventNames().submitEvent || event->type() == eventNames().resetEvent)) {
        event->stopPropagation();
        return;
    }
    HTMLElement::handleLocalEvents(event);
}
