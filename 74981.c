void LayerTilerChromium::invalidateTiles(const IntRect& oldLayerRect, const IntRect& newLayerRect)
{
    if (!m_tiles.size())
        return;

    IntRect oldContentRect = layerRectToContentRect(oldLayerRect);
    int oldLeft, oldTop, oldRight, oldBottom;
    contentRectToTileIndices(oldContentRect, oldLeft, oldTop, oldRight, oldBottom);

    IntRect newContentRect = layerRectToContentRect(newLayerRect);
    int newLeft, newTop, newRight, newBottom;
    contentRectToTileIndices(newContentRect, newLeft, newTop, newRight, newBottom);

    for (int j = oldTop; j <= oldBottom; ++j) {
        for (int i = oldLeft; i <= oldRight; ++i) {
            if (i >= newLeft && i <= newRight && j >= newTop && j <= newBottom)
                continue;

            const int index = tileIndex(i, j);
            if (m_tiles[index])
                m_unusedTiles.append(m_tiles[index].release());
        }
    }
}
