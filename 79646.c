bool HTMLFormControlElement::willValidate() const
{
    if (!m_willValidateInitialized || m_dataListAncestorState == Unknown) {
        m_willValidateInitialized = true;
        bool newWillValidate = recalcWillValidate();
        if (m_willValidate != newWillValidate) {
            m_willValidate = newWillValidate;
            const_cast<HTMLFormControlElement*>(this)->setNeedsValidityCheck();
        }
    } else {
        ASSERT(m_willValidate == recalcWillValidate());
    }
    return m_willValidate;
}
