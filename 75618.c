static void needsStyleRecalcCallback(Node* node, unsigned data)
{
    node->setNeedsStyleRecalc(static_cast<StyleChangeType>(data));
}
