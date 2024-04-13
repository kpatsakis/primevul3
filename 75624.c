bool ContainerNode::removeChild(Node* oldChild, ExceptionCode& ec)
{
    ASSERT(refCount() || parentOrHostNode());

    RefPtr<Node> protect(this);

    ec = 0;

    if (isReadOnlyNode()) {
        ec = NO_MODIFICATION_ALLOWED_ERR;
        return false;
    }

    if (!oldChild || oldChild->parentNode() != this) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    RefPtr<Node> child = oldChild;
    willRemoveChild(child.get());

    if (child->parentNode() != this) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    document()->removeFocusedNodeOfSubtree(child.get());

#if ENABLE(FULLSCREEN_API)
    document()->removeFullScreenElementOfSubtree(child.get());
#endif

    if (child->parentNode() != this) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    Node* prev = child->previousSibling();
    Node* next = child->nextSibling();
    removeBetween(prev, next, child.get());

    childrenChanged(false, prev, next, -1);

    ChildNodeRemovalNotifier(this).notify(child.get());
    dispatchSubtreeModifiedEvent();

    return child;
}
