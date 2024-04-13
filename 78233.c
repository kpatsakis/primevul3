bool HTMLFormElement::checkValidityWithoutDispatchingEvents()
{
    return !checkInvalidControlsAndCollectUnhandled(0, HTMLFormControlElement::CheckValidityDispatchEventsNone);
}
