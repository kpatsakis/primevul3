bool HTMLFormElement::rendererIsNeeded(const RenderStyle& style)
{
    if (!m_wasDemoted)
        return HTMLElement::rendererIsNeeded(style);

    ContainerNode* node = parentNode();
    RenderObject* parentRenderer = node->renderer();
    bool parentIsTableElementPart = (parentRenderer->isTable() && isHTMLTableElement(node))
        || (parentRenderer->isTableRow() && node->hasTagName(trTag))
        || (parentRenderer->isTableSection() && node->hasTagName(tbodyTag))
        || (parentRenderer->isRenderTableCol() && node->hasTagName(colTag))
        || (parentRenderer->isTableCell() && node->hasTagName(trTag));

    if (!parentIsTableElementPart)
        return true;

    EDisplay display = style.display();
    bool formIsTablePart = display == TABLE || display == INLINE_TABLE || display == TABLE_ROW_GROUP
        || display == TABLE_HEADER_GROUP || display == TABLE_FOOTER_GROUP || display == TABLE_ROW
        || display == TABLE_COLUMN_GROUP || display == TABLE_COLUMN || display == TABLE_CELL
        || display == TABLE_CAPTION;

    return formIsTablePart;
}
