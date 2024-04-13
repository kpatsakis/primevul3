void HTMLTextAreaElement::updateFocusAppearance(bool restorePreviousSelection)
{
    if (!restorePreviousSelection || !hasCachedSelection()) {
        setSelectionRange(0, 0);
    } else
        restoreCachedSelection();

    if (document().frame())
        document().frame()->selection().revealSelection();
}
