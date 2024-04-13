void LayerTilerChromium::draw(const IntRect& contentRect)
{
    if (m_skipsDraw || !m_tiles.size())
        return;

    GraphicsContext3D* context = layerRendererContext();
    const ContentLayerChromium::SharedValues* contentLayerValues = layerRenderer()->contentLayerSharedValues();
    layerRenderer()->useShader(contentLayerValues->contentShaderProgram());
    GLC(context, context->uniform1i(contentLayerValues->shaderSamplerLocation(), 0));

    int left, top, right, bottom;
    contentRectToTileIndices(contentRect, left, top, right, bottom);
    for (int j = top; j <= bottom; ++j) {
        for (int i = left; i <= right; ++i) {
            Tile* tile = m_tiles[tileIndex(i, j)].get();
            ASSERT(tile);

            tile->texture()->bindTexture();

            TransformationMatrix tileMatrix;
            IntRect tileRect = tileContentRect(i, j);
            tileMatrix.translate3d(tileRect.x() - contentRect.x() + tileRect.width() / 2.0, tileRect.y() - contentRect.y() + tileRect.height() / 2.0, 0);

            LayerChromium::drawTexturedQuad(context, layerRenderer()->projectionMatrix(), tileMatrix, m_tileSize.width(), m_tileSize.height(), 1, contentLayerValues->shaderMatrixLocation(), contentLayerValues->shaderAlphaLocation());

            tile->texture()->unreserve();
        }
    }
}
