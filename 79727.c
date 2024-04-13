void HTMLSelectElement::dispatchBlurEvent(Element* newFocusedElement)
{
    if (usesMenuList())
        dispatchChangeEventForMenuList();
    HTMLFormControlElementWithState::dispatchBlurEvent(newFocusedElement);
}
