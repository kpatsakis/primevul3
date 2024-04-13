void HTMLButtonElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == typeAttr) {
        if (equalIgnoringCase(value, "reset"))
            m_type = RESET;
        else if (equalIgnoringCase(value, "button"))
            m_type = BUTTON;
        else
            m_type = SUBMIT;
        setNeedsWillValidateCheck();
    } else
        HTMLFormControlElement::parseAttribute(name, value);
}
