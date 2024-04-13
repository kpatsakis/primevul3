static inline bool shouldIgnoreAttributeCase(const Element* e)
{
    return e && e->document()->isHTMLDocument() && e->isHTMLElement();
}
