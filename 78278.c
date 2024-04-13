void ConvolverNode::reset()
{
    MutexLocker locker(m_processLock);
    if (m_reverb.get())
        m_reverb->reset();
}
