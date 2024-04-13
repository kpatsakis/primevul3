void HTMLInputElement::detach(const AttachContext& context)
{
    HTMLTextFormControlElement::detach(context);
    setFormControlValueMatchesRenderer(false);
    m_inputTypeView->closePopupView();
}
