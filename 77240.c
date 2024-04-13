bool HarfBuzzShaperBase::isWordEnd(unsigned index)
{
    return index && isCodepointSpace(m_normalizedBuffer[index]) && !isCodepointSpace(m_normalizedBuffer[index - 1]);
}
