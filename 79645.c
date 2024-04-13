void HTMLFormControlElement::willCallDefaultEventHandler(const Event& event)
{
    if (!event.isKeyboardEvent() || event.type() != EventTypeNames::keydown)
        return;
    if (!m_wasFocusedByMouse)
        return;
    m_wasFocusedByMouse = false;
    if (renderer())
        renderer()->repaint();
}
