void Element::cancelFocusAppearanceUpdate()
{
    if (hasRareData())
        elementRareData()->setNeedsFocusAppearanceUpdateSoonAfterAttach(false);
    if (document()->focusedNode() == this)
        document()->cancelFocusAppearanceUpdate();
}
