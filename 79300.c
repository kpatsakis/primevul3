void TextIterator::emitText(Node* textNode, int textStartOffset, int textEndOffset)
{
    emitText(textNode, m_node->renderer(), textStartOffset, textEndOffset);
}
