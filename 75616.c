bool ContainerNode::insertBefore(PassRefPtr<Node> newChild, Node* refChild, ExceptionCode& ec, bool shouldLazyAttach)
{
    ASSERT(refCount() || parentOrHostNode());

    RefPtr<Node> protect(this);

    ec = 0;

    if (!refChild)
        return appendChild(newChild, ec, shouldLazyAttach);

    checkAddChild(newChild.get(), ec);
    if (ec)
        return false;

    if (refChild->parentNode() != this) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    if (refChild->previousSibling() == newChild || refChild == newChild) // nothing to do
        return true;

    RefPtr<Node> next = refChild;

    NodeVector targets;
    collectChildrenAndRemoveFromOldParent(newChild.get(), targets, ec);
    if (ec)
        return false;
    if (targets.isEmpty())
        return true;

    InspectorInstrumentation::willInsertDOMNode(document(), this);

#if ENABLE(MUTATION_OBSERVERS)
    ChildListMutationScope mutation(this);
#endif

    for (NodeVector::const_iterator it = targets.begin(); it != targets.end(); ++it) {
        Node* child = it->get();

        if (next->parentNode() != this)
            break;
        if (child->parentNode())
            break;

        treeScope()->adoptIfNeeded(child);

        insertBeforeCommon(next.get(), child);

        updateTreeAfterInsertion(this, child, shouldLazyAttach);
    }

    dispatchSubtreeModifiedEvent();
    return true;
}
