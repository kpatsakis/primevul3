bool HTMLFormElement::checkValidity()
{
    Vector<RefPtr<FormAssociatedElement> > controls;
    return !checkInvalidControlsAndCollectUnhandled(&controls);
}
