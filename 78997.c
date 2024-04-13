ShadowRoot* Element::shadowRoot() const
{
    ElementShadow* elementShadow = shadow();
    if (!elementShadow)
        return 0;
    ShadowRoot* shadowRoot = elementShadow->youngestShadowRoot();
    if (shadowRoot->type() == ShadowRoot::AuthorShadowRoot)
        return shadowRoot;
    return 0;
}
