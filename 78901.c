bool Element::isInert() const
{
    const Element* dialog = document()->activeModalDialog();
    if (dialog && !containsIncludingShadowDOM(dialog) && !dialog->containsIncludingShadowDOM(this))
        return true;
    return document()->ownerElement() && document()->ownerElement()->isInert();
}
