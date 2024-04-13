EditorClientBlackBerry::EditorClientBlackBerry(WebPagePrivate* webPagePrivate)
    : m_webPagePrivate(webPagePrivate)
    , m_waitingForCursorFocus(false)
    , m_spellCheckState(SpellCheckDefault)
    , m_inRedo(false)
{
}
