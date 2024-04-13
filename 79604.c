HTMLFormControlElement::HTMLFormControlElement(const QualifiedName& tagName, Document& document, HTMLFormElement* form)
    : LabelableElement(tagName, document)
    , m_disabled(false)
    , m_isAutofilled(false)
    , m_isReadOnly(false)
    , m_isRequired(false)
    , m_valueMatchesRenderer(false)
    , m_ancestorDisabledState(AncestorDisabledStateUnknown)
    , m_dataListAncestorState(Unknown)
    , m_willValidateInitialized(false)
    , m_willValidate(true)
    , m_isValid(true)
    , m_wasChangedSinceLastFormControlChangeEvent(false)
    , m_wasFocusedByMouse(false)
    , m_hasAutofocused(false)
{
    setForm(form ? form : findFormAncestor());
    setHasCustomStyleCallbacks();
}
