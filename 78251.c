void HTMLFormElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == actionAttr)
        m_attributes.parseAction(value);
    else if (name == targetAttr)
        m_attributes.setTarget(value);
    else if (name == methodAttr)
        m_attributes.updateMethodType(value);
    else if (name == enctypeAttr)
        m_attributes.updateEncodingType(value);
    else if (name == accept_charsetAttr)
        m_attributes.setAcceptCharset(value);
    else if (name == onautocompleteAttr)
        setAttributeEventListener(eventNames().autocompleteEvent, createAttributeEventListener(this, name, value));
    else if (name == onautocompleteerrorAttr)
        setAttributeEventListener(eventNames().autocompleteerrorEvent, createAttributeEventListener(this, name, value));
    else
        HTMLElement::parseAttribute(name, value);
}
