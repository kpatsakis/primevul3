    int readOutBytes(char* outputBuffer, unsigned askedToRead) {
        unsigned bytesLeft = m_buffer.size() - m_currentOffset;
        unsigned lenToCopy = min(askedToRead, bytesLeft);
        if (lenToCopy) {
            memcpy(outputBuffer, m_buffer.data() + m_currentOffset, lenToCopy);
            m_currentOffset += lenToCopy;
        }
        return lenToCopy;
    }
