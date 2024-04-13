Document* XSLStyleSheet::ownerDocument()
{
    for (XSLStyleSheet* styleSheet = this; styleSheet; styleSheet = styleSheet->parentStyleSheet()) {
        Node* node = styleSheet->ownerNode();
        if (node)
            return node->document();
    }
    return 0;
}
