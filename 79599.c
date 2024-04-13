bool HTMLButtonElement::recalcWillValidate() const
{
    return m_type == SUBMIT && HTMLFormControlElement::recalcWillValidate();
}
