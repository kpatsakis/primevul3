void Element::finishParsingChildren()
{
    ContainerNode::finishParsingChildren();
    setIsParsingChildrenFinished();
    checkForSiblingStyleChanges(this, renderStyle(), true, lastChild(), 0, 0);
    if (StyleResolver* styleResolver = document()->styleResolverIfExists())
        styleResolver->popParentElement(this);
}
