HTMLInputElement::~HTMLInputElement()
{
    setForm(0);
    if (isRadioButton())
        document().formController()->checkedRadioButtons().removeButton(this);
    if (m_hasTouchEventHandler)
        document().didRemoveEventTargetNode(this);
}
