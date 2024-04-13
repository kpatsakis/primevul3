bool HTMLInputElement::rendererIsNeeded(const RenderStyle& style)
{
    return m_inputType->rendererIsNeeded() && HTMLTextFormControlElement::rendererIsNeeded(style);
}
