    static void checkConsistency(const WebCore::ImageLoader* p)
    {
        if (!p)
            return;
        ASSERT(p->element());
        ValueCheck<WebCore::Element*>::checkConsistency(p->element());
    }
