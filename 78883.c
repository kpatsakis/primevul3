InputMethodContext* Element::getInputContext()
{
    return ensureElementRareData()->ensureInputMethodContext(toHTMLElement(this));
}
