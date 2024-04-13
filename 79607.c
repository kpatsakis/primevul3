bool HTMLFormControlElement::checkValidity(Vector<RefPtr<FormAssociatedElement> >* unhandledInvalidControls, CheckValidityDispatchEvents dispatchEvents)
{
    if (!willValidate() || isValidFormControlElement())
        return true;
    if (dispatchEvents == CheckValidityDispatchEventsNone)
        return false;
    RefPtr<HTMLFormControlElement> protector(this);
    RefPtr<Document> originalDocument(document());
    bool needsDefaultAction = dispatchEvent(Event::createCancelable(EventTypeNames::invalid));
    if (needsDefaultAction && unhandledInvalidControls && inDocument() && originalDocument == document())
        unhandledInvalidControls->append(this);
    return false;
}
