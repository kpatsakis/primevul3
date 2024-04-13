void ContainerNode::parserInsertBefore(PassRefPtr<Node> newChild, Node* nextChild)
{
    ASSERT(newChild);
    ASSERT(nextChild);
    ASSERT(nextChild->parentNode() == this);

    NodeVector targets;
    collectTargetNodes(newChild.get(), targets);
    if (targets.isEmpty())
        return;

    if (nextChild->previousSibling() == newChild || nextChild == newChild) // nothing to do
        return;

    RefPtr<Node> next = nextChild;
    RefPtr<Node> nextChildPreviousSibling = nextChild->previousSibling();
    for (NodeVector::const_iterator it = targets.begin(); it != targets.end(); ++it) {
        Node* child = it->get();

        insertBeforeCommon(next.get(), child);

        childrenChanged(true, nextChildPreviousSibling.get(), nextChild, 1);
        ChildNodeInsertionNotifier(this).notify(child);
    }
}
