AudioBuffer* ConvolverNode::buffer()
{
    ASSERT(isMainThread());
    return m_buffer.get();
}
