void HTMLButtonElement::defaultEventHandler(Event* event)
{
    if (event->type() == EventTypeNames::DOMActivate && !isDisabledFormControl()) {
        if (form() && m_type == SUBMIT) {
            m_isActivatedSubmit = true;
            form()->prepareForSubmission(event);
            event->setDefaultHandled();
            m_isActivatedSubmit = false; // Do this in case submission was canceled.
        }
        if (form() && m_type == RESET) {
            form()->reset();
            event->setDefaultHandled();
        }
    }

    if (event->isKeyboardEvent()) {
        if (event->type() == EventTypeNames::keydown && toKeyboardEvent(event)->keyIdentifier() == "U+0020") {
            setActive(true);
            return;
        }
        if (event->type() == EventTypeNames::keypress) {
            switch (toKeyboardEvent(event)->charCode()) {
                case '\r':
                    dispatchSimulatedClick(event);
                    event->setDefaultHandled();
                    return;
                case ' ':
                    event->setDefaultHandled();
                    return;
            }
        }
        if (event->type() == EventTypeNames::keyup && toKeyboardEvent(event)->keyIdentifier() == "U+0020") {
            if (active())
                dispatchSimulatedClick(event);
            event->setDefaultHandled();
            return;
        }
    }

    HTMLFormControlElement::defaultEventHandler(event);
}
