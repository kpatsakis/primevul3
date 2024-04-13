void ContainerNode::takeAllChildrenFrom(ContainerNode* oldParent)
{
    NodeVector children;
    getChildNodes(oldParent, children);
    oldParent->removeAllChildren();

    for (unsigned i = 0; i < children.size(); ++i) {
        ExceptionCode ec = 0;
        if (children[i]->attached())
            children[i]->detach();
        RefPtr<Node> child = document()->adoptNode(children[i].release(), ec);
        ASSERT(!ec);
        parserAddChild(child.get());
        treeScope()->adoptIfNeeded(child.get());
        if (attached() && !child->attached())
            child->attach();
    }
}
