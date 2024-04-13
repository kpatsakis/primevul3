KURL HTMLInputElement::src() const
{
    return document().completeURL(fastGetAttribute(srcAttr));
}
