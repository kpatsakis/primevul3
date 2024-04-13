void TextIterator::emitText(Node* textNode, RenderObject* renderObject, int textStartOffset, int textEndOffset)
{
    RenderText* renderer = toRenderText(renderObject);
    m_text = m_emitsOriginalText ? renderer->originalText() : (m_emitsTextWithoutTranscoding ? renderer->textWithoutTranscoding() : renderer->text());
    ASSERT(!m_text.isEmpty());
    ASSERT(0 <= textStartOffset && textStartOffset < static_cast<int>(m_text.length()));
    ASSERT(0 <= textEndOffset && textEndOffset <= static_cast<int>(m_text.length()));
    ASSERT(textStartOffset <= textEndOffset);

    m_positionNode = textNode;
    m_positionOffsetBaseNode = 0;
    m_positionStartOffset = textStartOffset;
    m_positionEndOffset = textEndOffset;
    m_textCharacters = 0;
    m_textLength = textEndOffset - textStartOffset;
    m_lastCharacter = m_text[textEndOffset - 1];

    m_lastTextNodeEndedWithCollapsedSpace = false;
    m_hasEmitted = true;
}
