HTMLTextAreaElement::HTMLTextAreaElement(Document& document, HTMLFormElement* form)
    : HTMLTextFormControlElement(textareaTag, document, form)
    , m_rows(defaultRows)
    , m_cols(defaultCols)
    , m_wrap(SoftWrap)
    , m_isDirty(false)
{
    setFormControlValueMatchesRenderer(true);
    ScriptWrappable::init(this);
}
