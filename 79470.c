    CompositingRecursionData(const CompositingRecursionData& other)
        : m_compositingAncestor(other.m_compositingAncestor)
        , m_mostRecentCompositedLayer(other.m_mostRecentCompositedLayer)
        , m_subtreeIsCompositing(other.m_subtreeIsCompositing)
        , m_hasUnisolatedCompositedBlendingDescendant(other.m_hasUnisolatedCompositedBlendingDescendant)
        , m_testingOverlap(other.m_testingOverlap)
#ifndef NDEBUG
        , m_depth(other.m_depth + 1)
#endif
    {
    }
