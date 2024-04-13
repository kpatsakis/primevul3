void BitStack::push(bool bit)
{
    unsigned index = m_size / bitsInWord;
    unsigned shift = m_size & bitInWordMask;
    if (!shift && index == m_words.size()) {
        m_words.grow(index + 1);
        m_words[index] = 0;
    }
    unsigned& word = m_words[index];
    unsigned mask = 1U << shift;
    if (bit)
        word |= mask;
    else
        word &= ~mask;
    ++m_size;
}
