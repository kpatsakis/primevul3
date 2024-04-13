SimplifiedBackwardsTextIterator::SimplifiedBackwardsTextIterator(const Range* r, TextIteratorBehavior behavior)
    : m_node(0)
    , m_offset(0)
    , m_handledNode(false)
    , m_handledChildren(false)
    , m_startNode(0)
    , m_startOffset(0)
    , m_endNode(0)
    , m_endOffset(0)
    , m_positionNode(0)
    , m_positionStartOffset(0)
    , m_positionEndOffset(0)
    , m_textCharacters(0)
    , m_textLength(0)
    , m_lastTextNode(0)
    , m_lastCharacter(0)
    , m_singleCharacterBuffer(0)
    , m_havePassedStartNode(false)
    , m_shouldHandleFirstLetter(false)
    , m_stopsOnFormControls(behavior & TextIteratorStopsOnFormControls)
    , m_shouldStop(false)
    , m_emitsOriginalText(false)
{
    ASSERT(behavior == TextIteratorDefaultBehavior || behavior == TextIteratorStopsOnFormControls);

    if (!r)
        return;

    Node* startNode = r->startContainer();
    if (!startNode)
        return;
    Node* endNode = r->endContainer();
    int startOffset = r->startOffset();
    int endOffset = r->endOffset();

    if (!startNode->offsetInCharacters()) {
        if (startOffset >= 0 && startOffset < static_cast<int>(startNode->childNodeCount())) {
            startNode = startNode->childNode(startOffset);
            startOffset = 0;
        }
    }
    if (!endNode->offsetInCharacters()) {
        if (endOffset > 0 && endOffset <= static_cast<int>(endNode->childNodeCount())) {
            endNode = endNode->childNode(endOffset - 1);
            endOffset = lastOffsetInNode(endNode);
        }
    }

    m_node = endNode;
    setUpFullyClippedStack(m_fullyClippedStack, m_node);    
    m_offset = endOffset;
    m_handledNode = false;
    m_handledChildren = endOffset == 0;

    m_startNode = startNode;
    m_startOffset = startOffset;
    m_endNode = endNode;
    m_endOffset = endOffset;
    
#ifndef NDEBUG
    m_positionNode = endNode;
#endif

    m_lastTextNode = 0;
    m_lastCharacter = '\n';

    m_havePassedStartNode = false;

    advance();
}
