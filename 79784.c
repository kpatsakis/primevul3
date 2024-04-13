String HTMLTextAreaElement::value() const
{
    updateValue();
    return m_value;
}
