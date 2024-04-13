static void processingInstructionHandler(void* closure, const xmlChar* target, const xmlChar* data)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;
    
    getTokenizer(closure)->processingInstruction(target, data);
}
