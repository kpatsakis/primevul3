void XMLTokenizer::error(ErrorType type, const char* message, va_list args)
{
    if (m_parserStopped)
        return;

#if PLATFORM(WIN_OS)
    char m[1024];
    vsnprintf(m, sizeof(m) - 1, message, args);
#else
    char* m;
    if (vasprintf(&m, message, args) == -1)
        return;
#endif
    
    if (m_parserPaused)
        m_pendingCallbacks->appendErrorCallback(type, m, lineNumber(), columnNumber());
    else
        handleError(type, m, lineNumber(), columnNumber());

#if !PLATFORM(WIN_OS)
    free(m);
#endif
}
