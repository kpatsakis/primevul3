static void needsSyntheticStyleChangeCallback(Node* node)
{
    node->setNeedsStyleRecalc(SyntheticStyleChange);
}
