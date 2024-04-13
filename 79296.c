static unsigned depthCrossingShadowBoundaries(Node* node)
{
    unsigned depth = 0;
    for (Node* parent = node->parentOrShadowHostNode(); parent; parent = parent->parentOrShadowHostNode())
        ++depth;
    return depth;
}
