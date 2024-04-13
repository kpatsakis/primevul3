void ContainerNode::removeChildren()
{
    if (!m_firstChild)
        return;

    RefPtr<ContainerNode> protect(this);

    willRemoveChildren(protect.get());

    document()->removeFocusedNodeOfSubtree(this, true);

#if ENABLE(FULLSCREEN_API)
    document()->removeFullScreenElementOfSubtree(this, true);
#endif

    forbidEventDispatch();
    Vector<RefPtr<Node>, 10> removedChildren;
    removedChildren.reserveInitialCapacity(childNodeCount());
    while (RefPtr<Node> n = m_firstChild) {
        Node* next = n->nextSibling();

        n->setPreviousSibling(0);
        n->setNextSibling(0);
        n->setParentOrHostNode(0);
        document()->adoptIfNeeded(n.get());

        m_firstChild = next;
        if (n == m_lastChild)
            m_lastChild = 0;
        removedChildren.append(n.release());
    }

    size_t removedChildrenCount = removedChildren.size();
    size_t i;

    for (i = 0; i < removedChildrenCount; ++i) {
        Node* removedChild = removedChildren[i].get();
        if (removedChild->attached())
            removedChild->detach();
    }

    childrenChanged(false, 0, 0, -static_cast<int>(removedChildrenCount));

    for (i = 0; i < removedChildrenCount; ++i)
        ChildNodeRemovalNotifier(this).notify(removedChildren[i].get());

    allowEventDispatch();
    dispatchSubtreeModifiedEvent();
}
