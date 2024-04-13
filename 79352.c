static void setUpFullyClippedStack(BitStack& stack, Node* node)
{
    Vector<Node*, 100> ancestry;
    for (Node* parent = node->parentOrShadowHostNode(); parent; parent = parent->parentOrShadowHostNode())
        ancestry.append(parent);

    size_t size = ancestry.size();
    for (size_t i = 0; i < size; ++i)
        pushFullyClippedState(stack, ancestry[size - i - 1]);
    pushFullyClippedState(stack, node);

    ASSERT(stack.size() == 1 + depthCrossingShadowBoundaries(node));
}
