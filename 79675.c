void* HTMLInputElement::preDispatchEventHandler(Event* event)
{
    if (event->type() == EventTypeNames::textInput && m_inputTypeView->shouldSubmitImplicitly(event)) {
        event->stopPropagation();
        return 0;
    }
    if (event->type() != EventTypeNames::click)
        return 0;
    if (!event->isMouseEvent() || toMouseEvent(event)->button() != LeftButton)
        return 0;
    return m_inputTypeView->willDispatchClick().leakPtr();
}
