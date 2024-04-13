static bool shouldEmitTabBeforeNode(Node* node)
{
    RenderObject* r = node->renderer();
    
    if (!r || !isTableCell(node))
        return false;
    
    RenderTableCell* rc = toRenderTableCell(r);
    RenderTable* t = rc->table();
    return t && (t->cellBefore(rc) || t->cellAbove(rc));
}
