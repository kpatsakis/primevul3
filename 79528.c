    bool overlapsLayers(const IntRect& bounds) const
    {
        return m_overlapStack.last().overlapsLayers(bounds);
    }
