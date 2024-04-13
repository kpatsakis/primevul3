    void add(const RenderLayer* layer, const IntRect& bounds)
    {
        ASSERT(m_overlapStack.size() >= 2);
        m_overlapStack[m_overlapStack.size() - 2].add(bounds);
        m_layers.add(layer);
    }
