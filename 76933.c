void EditorClientBlackBerry::toggleContinuousSpellChecking()
{
    if (isContinuousSpellCheckingEnabled())
        m_spellCheckState = SpellCheckOff;
    else
        m_spellCheckState = SpellCheckOn;
}
