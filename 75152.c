unsigned Cache::deadCapacity() const 
{
    unsigned capacity = m_capacity - min(m_liveSize, m_capacity); // Start with available capacity.
    capacity = max(capacity, m_minDeadCapacity); // Make sure it's above the minimum.
    capacity = min(capacity, m_maxDeadCapacity); // Make sure it's below the maximum.
    return capacity;
}
