void ContainerNode::childrenChanged(bool changedByParser, Node*, Node*, int childCountDelta)
{
    document()->incDOMTreeVersion();
    if (!changedByParser && childCountDelta)
        document()->updateRangesAfterChildrenChanged(this);
    invalidateNodeListCachesInAncestors();
}
