bool ContainerNode::appendChild(PassRefPtr<Node> newChild, ExceptionCode& ec, bool shouldLazyAttach)
{
    RefPtr<ContainerNode> protect(this);

    ASSERT(refCount() || parentOrHostNode());

    ec = 0;

    checkAddChild(newChild.get(), ec);
    if (ec)
        return false;

    if (newChild == m_lastChild) // nothing to do
        return newChild;

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

        if (child->parentNode())
            break;

        treeScope()->adoptIfNeeded(child);

        forbidEventDispatch();
        appendChildToContainer(child, this);
        allowEventDispatch();

        updateTreeAfterInsertion(this, child, shouldLazyAttach);
    }

    dispatchSubtreeModifiedEvent();
    return true;
}
