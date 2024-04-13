PassRefPtr<Range> BackwardsCharacterIterator::range() const
{
    RefPtr<Range> r = m_textIterator.range();
    if (!m_textIterator.atEnd()) {
        if (m_textIterator.length() <= 1)
            ASSERT(m_runOffset == 0);
        else {
            Node* n = r->startContainer();
            ASSERT(n == r->endContainer());
            int offset = r->endOffset() - m_runOffset;
            r->setStart(n, offset - 1, ASSERT_NO_EXCEPTION);
            r->setEnd(n, offset, ASSERT_NO_EXCEPTION);
        }
    }
    return r.release();
}
