    void setEmpty(bool empty, RenderBlock* block = 0, LineWidth* lineWidth = 0)
    {
        if (m_isEmpty == empty)
            return;
        m_isEmpty = empty;
        if (!empty && block && floatPaginationStrut()) {
            block->setLogicalHeight(block->logicalHeight() + floatPaginationStrut());
            setFloatPaginationStrut(0);
            lineWidth->updateAvailableWidth();
        }
    }
