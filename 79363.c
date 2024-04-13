bool BitStack::top() const
{
    if (!m_size)
        return false;
    unsigned shift = (m_size - 1) & bitInWordMask;
    return m_words.last() & (1U << shift);
}
