    void unite(const OverlapMapContainer& otherContainer)
    {
        m_layerRects.append(otherContainer.m_layerRects);
        m_boundingBox.unite(otherContainer.m_boundingBox);
    }
