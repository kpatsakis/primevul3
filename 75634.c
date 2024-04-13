static void willRemoveChildren(ContainerNode* container)
{
    NodeVector children;
    getChildNodes(container, children);

    container->document()->nodeChildrenWillBeRemoved(container);

#if ENABLE(MUTATION_OBSERVERS)
    ChildListMutationScope mutation(container);
#endif

    for (NodeVector::const_iterator it = children.begin(); it != children.end(); it++) {
        Node* child = it->get();

#if ENABLE(MUTATION_OBSERVERS)
        mutation.willRemoveChild(child);
        child->notifyMutationObserversNodeWillDetach();
#endif

        dispatchChildRemovalEvents(child);
        ChildFrameDisconnector(child).disconnect();
    }
}
