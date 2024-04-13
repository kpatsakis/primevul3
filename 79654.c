void HTMLInputElement::copyNonAttributePropertiesFromElement(const Element& source)
{
    const HTMLInputElement& sourceElement = static_cast<const HTMLInputElement&>(source);

    m_valueIfDirty = sourceElement.m_valueIfDirty;
    setChecked(sourceElement.m_isChecked);
    m_reflectsCheckedAttribute = sourceElement.m_reflectsCheckedAttribute;
    m_isIndeterminate = sourceElement.m_isIndeterminate;

    HTMLTextFormControlElement::copyNonAttributePropertiesFromElement(source);

    setFormControlValueMatchesRenderer(false);
    m_inputTypeView->updateView();
}
