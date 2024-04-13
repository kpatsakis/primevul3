static void collectTargetNodes(Node* node, NodeVector& nodes)
{
    if (node->nodeType() != Node::DOCUMENT_FRAGMENT_NODE) {
        nodes.append(node);
        return;
    }
    getChildNodes(node, nodes);
}
