static void normalErrorHandler(void* closure, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    getTokenizer(closure)->error(XMLTokenizer::nonFatal, message, args);
    va_end(args);
}
