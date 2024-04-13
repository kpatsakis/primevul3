void TextIterator::emitCharacter(UChar c, Node* textNode, Node* offsetBaseNode, int textStartOffset, int textEndOffset)
{
    m_hasEmitted = true;
    
    m_positionNode = textNode;
    m_positionOffsetBaseNode = offsetBaseNode;
    m_positionStartOffset = textStartOffset;
    m_positionEndOffset = textEndOffset;
 
    m_singleCharacterBuffer = c;
    m_textCharacters = &m_singleCharacterBuffer;
    m_textLength = 1;

    m_lastTextNodeEndedWithCollapsedSpace = false;
    m_lastCharacter = c;
}
