void Element::blur()
{
    cancelFocusAppearanceUpdate();
    Document* doc = document();
    if (treeScope()->focusedNode() == this) {
        if (doc->frame())
            doc->frame()->page()->focusController()->setFocusedNode(0, doc->frame());
        else
            doc->setFocusedNode(0);
    }
}
