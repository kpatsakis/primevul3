void ContainerNode::detach()
{
    detachChildren();
    clearChildNeedsStyleRecalc();
    Node::detach();
}
