RenderObject::SelectionState InlineTextBox::selectionState()
{
    RenderObject::SelectionState state = renderer()->selectionState();
    if (state == RenderObject::SelectionStart || state == RenderObject::SelectionEnd || state == RenderObject::SelectionBoth) {
        int startPos, endPos;
        renderer()->selectionStartEnd(startPos, endPos);
        int lastSelectable = start() + len() - (isLineBreak() ? 1 : 0);

        bool start = (state != RenderObject::SelectionEnd && startPos >= m_start && startPos < m_start + m_len);
        bool end = (state != RenderObject::SelectionStart && endPos > m_start && endPos <= lastSelectable);
        if (start && end)
            state = RenderObject::SelectionBoth;
        else if (start)
            state = RenderObject::SelectionStart;
        else if (end)
            state = RenderObject::SelectionEnd;
        else if ((state == RenderObject::SelectionEnd || startPos < m_start) &&
                 (state == RenderObject::SelectionStart || endPos > lastSelectable))
            state = RenderObject::SelectionInside;
        else if (state == RenderObject::SelectionBoth)
            state = RenderObject::SelectionNone;
    }

    if (m_truncation != cNoTruncation && root()->ellipsisBox()) {
        EllipsisBox* ellipsis = root()->ellipsisBox();
        if (state != RenderObject::SelectionNone) {
            int start, end;
            selectionStartEnd(start, end);
            ellipsis->setSelectionState(end >= m_truncation && start <= m_truncation ?
                RenderObject::SelectionInside : RenderObject::SelectionNone);
        } else
            ellipsis->setSelectionState(RenderObject::SelectionNone);
    }

    return state;
}
