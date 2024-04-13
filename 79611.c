void HTMLFormControlElement::dispatchBlurEvent(Element* newFocusedElement)
{
    HTMLElement::dispatchBlurEvent(newFocusedElement);
    hideVisibleValidationMessage();
}
