void ContainerNode::removeBetween(Node* previousChild, Node* nextChild, Node* oldChild)
{
    ASSERT(oldChild);
    ASSERT(oldChild->parentNode() == this);

    forbidEventDispatch();

    if (oldChild->attached())
        oldChild->detach();

    if (nextChild)
        nextChild->setPreviousSibling(previousChild);
    if (previousChild)
        previousChild->setNextSibling(nextChild);
    if (m_firstChild == oldChild)
        m_firstChild = nextChild;
    if (m_lastChild == oldChild)
        m_lastChild = previousChild;

    oldChild->setPreviousSibling(0);
    oldChild->setNextSibling(0);
    oldChild->setParentOrHostNode(0);

    document()->adoptIfNeeded(oldChild);

    allowEventDispatch();
}
