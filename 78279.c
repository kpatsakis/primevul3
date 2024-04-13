double ConvolverNode::tailTime() const
{
    MutexTryLocker tryLocker(m_processLock);
    if (tryLocker.locked())
        return m_reverb ? m_reverb->impulseResponseLength() / static_cast<double>(sampleRate()) : 0;
    return std::numeric_limits<double>::infinity();
}
