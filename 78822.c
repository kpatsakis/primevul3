void Element::beginParsingChildren()
{
    clearIsParsingChildrenFinished();
    StyleResolver* styleResolver = document()->styleResolverIfExists();
    if (styleResolver && attached())
        styleResolver->pushParentElement(this);
}
