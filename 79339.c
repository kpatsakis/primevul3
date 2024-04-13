static void pushFullyClippedState(BitStack& stack, Node* node)
{
    ASSERT(stack.size() == depthCrossingShadowBoundaries(node));

    stack.push(fullyClipsContents(node) || (stack.top() && !ignoresContainerClip(node)));
}
