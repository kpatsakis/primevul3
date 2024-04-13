    void kick()
    {
        if (!m_completion)
            return;
        Vector<WebTextCheckingResult> results;
        const int misspellingStartOffset = 1;
        const int misspellingLength = 8;
        results.append(WebTextCheckingResult(WebTextCheckingTypeSpelling, misspellingStartOffset, misspellingLength));
        m_completion->didFinishCheckingText(results);
        m_completion = 0;
    }
