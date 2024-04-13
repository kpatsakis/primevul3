void HTMLInputElement::resetImpl()
{
    if (m_inputType->storesValueSeparateFromAttribute())
        setValue(String());

    setChecked(hasAttribute(checkedAttr));
    m_reflectsCheckedAttribute = true;
}
