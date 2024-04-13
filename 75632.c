static void updateTreeAfterInsertion(ContainerNode* parent, Node* child, bool shouldLazyAttach)
{
    ASSERT(parent->refCount());
    ASSERT(child->refCount());

#if ENABLE(MUTATION_OBSERVERS)
    ChildListMutationScope(parent).childAdded(child);
#endif

    parent->childrenChanged(false, child->previousSibling(), child->nextSibling(), 1);

    ChildNodeInsertionNotifier(parent).notify(child);

    if (parent->attached() && !child->attached() && child->parentNode() == parent) {
        if (shouldLazyAttach)
            child->lazyAttach();
        else
            child->attach();
    }

    dispatchChildInsertionEvents(child);
}
