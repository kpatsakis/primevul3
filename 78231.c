bool HTMLFormElement::checkInvalidControlsAndCollectUnhandled(Vector<RefPtr<FormAssociatedElement> >* unhandledInvalidControls, HTMLFormControlElement::CheckValidityDispatchEvents dispatchEvents)
{
    RefPtr<HTMLFormElement> protector(this);
    Vector<RefPtr<FormAssociatedElement> > elements;
    elements.reserveCapacity(m_associatedElements.size());
    for (unsigned i = 0; i < m_associatedElements.size(); ++i)
        elements.append(m_associatedElements[i]);
    bool hasInvalidControls = false;
    for (unsigned i = 0; i < elements.size(); ++i) {
        if (elements[i]->form() == this && elements[i]->isFormControlElement()) {
            HTMLFormControlElement* control = toHTMLFormControlElement(elements[i].get());
            if (!control->checkValidity(unhandledInvalidControls, dispatchEvents) && control->form() == this)
                hasInvalidControls = true;
        }
    }
    return hasInvalidControls;
}
