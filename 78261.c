void HTMLFormElement::reset()
{
    Frame* frame = document().frame();
    if (m_isInResetFunction || !frame)
        return;

    m_isInResetFunction = true;

    if (!dispatchEvent(Event::createCancelableBubble(eventNames().resetEvent))) {
        m_isInResetFunction = false;
        return;
    }

    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        if (m_associatedElements[i]->isFormControlElement())
            toHTMLFormControlElement(m_associatedElements[i])->reset();
    }

    m_isInResetFunction = false;
}
