void HTMLFormControlElement::dispatchFocusEvent(Element* oldFocusedElement, FocusDirection direction)
{
    if (direction != FocusDirectionPage)
        m_wasFocusedByMouse = direction == FocusDirectionMouse;
    HTMLElement::dispatchFocusEvent(oldFocusedElement, direction);
}
