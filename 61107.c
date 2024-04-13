static void djvu_close_lc(LoadContext* lc)
{
        if (lc->document)
                ddjvu_document_release(lc->document);
        if (lc->context)
                ddjvu_context_release(lc->context);
        if (lc->page)
                ddjvu_page_release(lc->page);
        RelinquishMagickMemory(lc);
}
