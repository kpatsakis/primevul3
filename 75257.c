bool TextAutosizer::isAutosizingCluster(const RenderBlock* renderer)
{
    ASSERT(isAutosizingContainer(renderer));

    return renderer->isRenderView()
        || renderer->isFloating()
        || renderer->isOutOfFlowPositioned()
        || renderer->isTableCell()
        || renderer->isTableCaption()
        || renderer->isFlexibleBoxIncludingDeprecated()
        || renderer->hasColumns()
        || renderer->style()->overflowX() != OVISIBLE
        || renderer->style()->overflowY() != OVISIBLE
        || renderer->containingBlock()->isHorizontalWritingMode() != renderer->isHorizontalWritingMode();
}
