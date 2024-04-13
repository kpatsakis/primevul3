void ContainerNode::setFocus(bool received)
{
    if (focused() == received)
        return;

    Node::setFocus(received);

    setNeedsStyleRecalc();
}
