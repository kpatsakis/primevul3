RenderText* SimplifiedBackwardsTextIterator::handleFirstLetter(int& startOffset, int& offsetInNode)
{
    RenderText* renderer = toRenderText(m_node->renderer());
    startOffset = (m_node == m_startNode) ? m_startOffset : 0;

    if (!renderer->isTextFragment()) {
        offsetInNode = 0;
        return renderer;
    }

    RenderTextFragment* fragment = toRenderTextFragment(renderer);
    int offsetAfterFirstLetter = fragment->start();
    if (startOffset >= offsetAfterFirstLetter) {
        ASSERT(!m_shouldHandleFirstLetter);
        offsetInNode = offsetAfterFirstLetter;
        return renderer;
    }

    if (!m_shouldHandleFirstLetter && offsetAfterFirstLetter < m_offset) {
        m_shouldHandleFirstLetter = true;
        offsetInNode = offsetAfterFirstLetter;
        return renderer;
    }

    m_shouldHandleFirstLetter = false;
    offsetInNode = 0;
    return firstRenderTextInFirstLetter(fragment->firstLetter());
}
