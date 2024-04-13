bool HTMLFormElement::validateInteractively(Event* event)
{
    ASSERT(event);
    if (!document().page() || noValidate())
        return true;

    HTMLFormControlElement* submitElement = submitElementFromEvent(event);
    if (submitElement && submitElement->formNoValidate())
        return true;

    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        if (m_associatedElements[i]->isFormControlElement())
            toHTMLFormControlElement(m_associatedElements[i])->hideVisibleValidationMessage();
    }

    Vector<RefPtr<FormAssociatedElement> > unhandledInvalidControls;
    if (!checkInvalidControlsAndCollectUnhandled(&unhandledInvalidControls))
        return true;

    document().updateLayoutIgnorePendingStylesheets();

    RefPtr<HTMLFormElement> protector(this);
    for (unsigned i = 0; i < unhandledInvalidControls.size(); ++i) {
        FormAssociatedElement* unhandledAssociatedElement = unhandledInvalidControls[i].get();
        HTMLElement* unhandled = toHTMLElement(unhandledAssociatedElement);
        if (unhandled->isFocusable() && unhandled->inDocument()) {
            unhandled->scrollIntoViewIfNeeded(false);
            unhandled->focus();
            if (unhandled->isFormControlElement())
                toHTMLFormControlElement(unhandled)->updateVisibleValidationMessage();
            break;
        }
    }
    if (document().frame()) {
        for (unsigned i = 0; i < unhandledInvalidControls.size(); ++i) {
            FormAssociatedElement* unhandledAssociatedElement = unhandledInvalidControls[i].get();
            HTMLElement* unhandled = toHTMLElement(unhandledAssociatedElement);
            if (unhandled->isFocusable() && unhandled->inDocument())
                continue;
            String message("An invalid form control with name='%name' is not focusable.");
            message.replace("%name", unhandledAssociatedElement->name());
            document().addConsoleMessage(RenderingMessageSource, ErrorMessageLevel, message);
        }
    }
    return false;
}
