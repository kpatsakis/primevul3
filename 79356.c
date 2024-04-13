static bool shouldEmitNewlinesBeforeAndAfterNode(Node* node)
{
    RenderObject* r = node->renderer();
    if (!r) {
        return (node->hasTagName(blockquoteTag)
                || node->hasTagName(ddTag)
                || node->hasTagName(divTag)
                || node->hasTagName(dlTag)
                || node->hasTagName(dtTag)
                || node->hasTagName(h1Tag)
                || node->hasTagName(h2Tag)
                || node->hasTagName(h3Tag)
                || node->hasTagName(h4Tag)
                || node->hasTagName(h5Tag)
                || node->hasTagName(h6Tag)
                || node->hasTagName(hrTag)
                || node->hasTagName(liTag)
                || node->hasTagName(listingTag)
                || node->hasTagName(olTag)
                || node->hasTagName(pTag)
                || node->hasTagName(preTag)
                || node->hasTagName(trTag)
                || node->hasTagName(ulTag));
    }
    
    if (isTableCell(node))
        return false;
    
    if (r->isTableRow()) {
        RenderTable* t = toRenderTableRow(r)->table();
        if (t && !t->isInline())
            return true;
    }
    
    return !r->isInline() && r->isRenderBlock()
        && !r->isFloatingOrOutOfFlowPositioned() && !r->isBody() && !r->isRubyText();
}
