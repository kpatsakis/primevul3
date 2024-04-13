void HTMLSelectElement::dispatchFocusEvent(Element* oldFocusedElement, FocusDirection direction)
{
    if (usesMenuList())
        saveLastSelection();
    HTMLFormControlElementWithState::dispatchFocusEvent(oldFocusedElement, direction);
}
