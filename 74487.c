static void warningHandler(void* closure, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    getTokenizer(closure)->error(XMLTokenizer::warning, message, args);
    va_end(args);
}
