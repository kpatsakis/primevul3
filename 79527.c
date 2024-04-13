    bool overlapsLayers(const IntRect& bounds) const
    {
        if (!bounds.intersects(m_boundingBox))
            return false;
        for (unsigned i = 0; i < m_layerRects.size(); i++) {
            if (m_layerRects[i].intersects(bounds))
                return true;
        }
        return false;
    }
