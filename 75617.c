void ContainerNode::insertBeforeCommon(Node* nextChild, Node* newChild)
{
    ASSERT(newChild);
    ASSERT(!newChild->parentNode()); // Use insertBefore if you need to handle reparenting (and want DOM mutation events).
    ASSERT(!newChild->nextSibling());
    ASSERT(!newChild->previousSibling());
    ASSERT(!newChild->isShadowRoot());

    forbidEventDispatch();
    Node* prev = nextChild->previousSibling();
    ASSERT(m_lastChild != prev);
    nextChild->setPreviousSibling(newChild);
    if (prev) {
        ASSERT(m_firstChild != nextChild);
        ASSERT(prev->nextSibling() == nextChild);
        prev->setNextSibling(newChild);
    } else {
        ASSERT(m_firstChild == nextChild);
        m_firstChild = newChild;
    }
    newChild->setParentOrHostNode(this);
    newChild->setPreviousSibling(prev);
    newChild->setNextSibling(nextChild);
    allowEventDispatch();
}
