bool EditorClientBlackBerry::isContinuousSpellCheckingEnabled()
{
    if (m_spellCheckState == SpellCheckOff)
        return false;
    if (m_spellCheckState == SpellCheckOn)
        return true;
    return shouldSpellCheckFocusedField();
}
