bool HTMLTextAreaElement::appendFormData(FormDataList& encoding, bool)
{
    if (name().isEmpty())
        return false;

    document().updateLayout();

    const String& text = (m_wrap == HardWrap) ? valueWithHardLineBreaks() : value();
    encoding.appendData(name(), text);

    const AtomicString& dirnameAttrValue = fastGetAttribute(dirnameAttr);
    if (!dirnameAttrValue.isNull())
        encoding.appendData(dirnameAttrValue, directionForFormData());
    return true;
}
