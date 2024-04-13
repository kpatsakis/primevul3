HTMLFormElement::HTMLFormElement(const QualifiedName& tagName, Document& document)
    : HTMLElement(tagName, document)
    , m_associatedElementsBeforeIndex(0)
    , m_associatedElementsAfterIndex(0)
    , m_wasUserSubmitted(false)
    , m_isSubmittingOrPreparingForSubmission(false)
    , m_shouldSubmit(false)
    , m_isInResetFunction(false)
    , m_wasDemoted(false)
    , m_requestAutocompleteTimer(this, &HTMLFormElement::requestAutocompleteTimerFired)
{
    ASSERT(hasTagName(formTag));
    ScriptWrappable::init(this);
}
