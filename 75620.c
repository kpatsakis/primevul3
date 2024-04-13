void ContainerNode::parserRemoveChild(Node* oldChild)
{
    ASSERT(oldChild);
    ASSERT(oldChild->parentNode() == this);

    Node* prev = oldChild->previousSibling();
    Node* next = oldChild->nextSibling();

    removeBetween(prev, next, oldChild);

    childrenChanged(true, prev, next, -1);
    ChildNodeRemovalNotifier(this).notify(oldChild);
}
