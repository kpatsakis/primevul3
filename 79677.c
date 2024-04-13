void HTMLInputElement::requiredAttributeChanged()
{
    HTMLTextFormControlElement::requiredAttributeChanged();
    if (CheckedRadioButtons* buttons = checkedRadioButtons())
        buttons->requiredAttributeChanged(this);
    m_inputTypeView->requiredAttributeChanged();
}
