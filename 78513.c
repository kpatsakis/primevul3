static inline BidiRun* createRun(int start, int end, RenderObject* obj, InlineBidiResolver& resolver)
{
    return new BidiRun(start, end, obj, resolver.context(), resolver.dir());
}
