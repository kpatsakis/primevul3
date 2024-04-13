HTMLSelectElement::HTMLSelectElement(Document& document, HTMLFormElement* form, bool createdByParser)
    : HTMLFormControlElementWithState(selectTag, document, form)
    , m_typeAhead(this)
    , m_size(0)
    , m_lastOnChangeIndex(-1)
    , m_activeSelectionAnchorIndex(-1)
    , m_activeSelectionEndIndex(-1)
    , m_isProcessingUserDrivenChange(false)
    , m_multiple(false)
    , m_activeSelectionState(false)
    , m_shouldRecalcListItems(false)
    , m_isParsingInProgress(createdByParser)
{
    ScriptWrappable::init(this);
}
