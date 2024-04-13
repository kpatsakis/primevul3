static bool shouldEmitNewlineForNode(Node* node, bool emitsOriginalText)
{
    RenderObject* renderer = node->renderer();

    if (renderer ? !renderer->isBR() : !node->hasTagName(brTag))
        return false;
    return emitsOriginalText || !(node->isInShadowTree() && node->shadowHost()->toInputElement());
}
