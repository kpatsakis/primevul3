void ContainerNode::cloneChildNodes(ContainerNode *clone)
{
    HTMLElement* deleteButtonContainerElement = 0;
    if (Frame* frame = document()->frame())
        deleteButtonContainerElement = frame->editor()->deleteButtonController()->containerElement();

    ExceptionCode ec = 0;
    for (Node* n = firstChild(); n && !ec; n = n->nextSibling()) {
        if (n == deleteButtonContainerElement)
            continue;
        clone->appendChild(n->cloneNode(true), ec);
    }
}
