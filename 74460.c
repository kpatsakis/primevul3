static void fatalErrorHandler(void* closure, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    getTokenizer(closure)->error(XMLTokenizer::fatal, message, args);
    va_end(args);
}
