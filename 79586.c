inline HTMLButtonElement::HTMLButtonElement(Document& document, HTMLFormElement* form)
    : HTMLFormControlElement(buttonTag, document, form)
    , m_type(SUBMIT)
    , m_isActivatedSubmit(false)
{
    ScriptWrappable::init(this);
}
