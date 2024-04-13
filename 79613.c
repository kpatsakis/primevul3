void HTMLFormControlElement::dispatchFormControlInputEvent()
{
    setChangedSinceLastFormControlChangeEvent(true);
    HTMLElement::dispatchInputEvent();
}
