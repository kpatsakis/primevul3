void Element::childrenChanged(bool changedByParser, Node* beforeChange, Node* afterChange, int childCountDelta)
{
    ContainerNode::childrenChanged(changedByParser, beforeChange, afterChange, childCountDelta);
    if (changedByParser)
        checkForEmptyStyleChange(this, renderStyle());
    else
        checkForSiblingStyleChanges(this, renderStyle(), false, beforeChange, afterChange, childCountDelta);

    if (ElementShadow * shadow = this->shadow())
        shadow->invalidateDistribution();
}
