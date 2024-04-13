static bool shouldEmitExtraNewlineForNode(Node* node)
{
    RenderObject* r = node->renderer();
    if (!r || !r->isBox())
        return false;
    
    if (node->hasTagName(h1Tag)
        || node->hasTagName(h2Tag)
        || node->hasTagName(h3Tag)
        || node->hasTagName(h4Tag)
        || node->hasTagName(h5Tag)
        || node->hasTagName(h6Tag)
        || node->hasTagName(pTag)) {
        RenderStyle* style = r->style();
        if (style) {
            int bottomMargin = toRenderBox(r)->collapsedMarginAfter();
            int fontSize = style->fontDescription().computedPixelSize();
            if (bottomMargin * 2 >= fontSize)
                return true;
        }
    }
    
    return false;
}
