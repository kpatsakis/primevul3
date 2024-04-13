WordAwareIterator::WordAwareIterator(const Range* r)
    : m_previousText(0)
    , m_didLookAhead(true) // so we consider the first chunk from the text iterator
    , m_textIterator(r)
{
    advance(); // get in position over the first chunk of text
}
