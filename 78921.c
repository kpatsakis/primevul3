PassRefPtr<RenderStyle> Element::originalStyleForRenderer()
{
    return document()->styleResolver()->styleForElement(this);
}
