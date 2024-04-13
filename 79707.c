bool HTMLInputElement::supportLabels() const
{
    return m_inputType->isInteractiveContent();
}
