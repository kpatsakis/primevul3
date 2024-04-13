void HTMLInputElement::defaultEventHandler(Event* evt)
{
    if (evt->isMouseEvent() && evt->type() == EventTypeNames::click && toMouseEvent(evt)->button() == LeftButton) {
        m_inputTypeView->handleClickEvent(toMouseEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isTouchEvent()) {
        m_inputTypeView->handleTouchEvent(toTouchEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == EventTypeNames::keydown) {
        m_inputTypeView->handleKeydownEvent(toKeyboardEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    bool callBaseClassEarly = isTextField() && (evt->type() == EventTypeNames::keydown || evt->type() == EventTypeNames::keypress);
    if (callBaseClassEarly) {
        HTMLTextFormControlElement::defaultEventHandler(evt);
        if (evt->defaultHandled())
            return;
    }

    if (evt->type() == EventTypeNames::DOMActivate) {
        m_inputType->handleDOMActivateEvent(evt);
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == EventTypeNames::keypress) {
        m_inputTypeView->handleKeypressEvent(toKeyboardEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == EventTypeNames::keyup) {
        m_inputTypeView->handleKeyupEvent(toKeyboardEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    if (m_inputTypeView->shouldSubmitImplicitly(evt)) {
        if (isSearchField())
            onSearch();
        if (wasChangedSinceLastFormControlChangeEvent())
            dispatchFormControlChangeEvent();

        RefPtr<HTMLFormElement> formForSubmission = m_inputTypeView->formForSubmission();
        if (formForSubmission)
            formForSubmission->submitImplicitly(evt, canTriggerImplicitSubmission());

        evt->setDefaultHandled();
        return;
    }

    if (evt->isBeforeTextInsertedEvent())
        m_inputTypeView->handleBeforeTextInsertedEvent(static_cast<BeforeTextInsertedEvent*>(evt));

    if (evt->isMouseEvent() && evt->type() == EventTypeNames::mousedown) {
        m_inputTypeView->handleMouseDownEvent(toMouseEvent(evt));
        if (evt->defaultHandled())
            return;
    }

    m_inputTypeView->forwardEvent(evt);

    if (!callBaseClassEarly && !evt->defaultHandled())
        HTMLTextFormControlElement::defaultEventHandler(evt);
}
