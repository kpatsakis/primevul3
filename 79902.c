bool ScriptLoader::prepareScript(const TextPosition& scriptStartPosition, LegacyTypeSupport supportLegacyTypes)
{
    if (m_alreadyStarted)
        return false;

    ScriptLoaderClient* client = this->client();

    bool wasParserInserted;
    if (m_parserInserted) {
        wasParserInserted = true;
        m_parserInserted = false;
    } else {
        wasParserInserted = false;
    }

    if (wasParserInserted && !client->asyncAttributeValue())
        m_forceAsync = true;

    if (!client->hasSourceAttribute() && !m_element->firstChild())
        return false;

    if (!m_element->inDocument())
        return false;

    if (!isScriptTypeSupported(supportLegacyTypes))
        return false;

    if (wasParserInserted) {
        m_parserInserted = true;
        m_forceAsync = false;
    }

    m_alreadyStarted = true;

    Document& elementDocument = m_element->document();
    Document* contextDocument = elementDocument.contextDocument().get();

    if (!contextDocument || !contextDocument->allowExecutingScripts(m_element))
        return false;

    if (!isScriptForEventSupported())
        return false;

    if (!client->charsetAttributeValue().isEmpty())
        m_characterEncoding = client->charsetAttributeValue();
    else
        m_characterEncoding = elementDocument.charset();

    if (client->hasSourceAttribute()) {
        if (!fetchScript(client->sourceAttributeValue()))
            return false;
    }

    if (client->hasSourceAttribute() && client->deferAttributeValue() && m_parserInserted && !client->asyncAttributeValue()) {
        m_willExecuteWhenDocumentFinishedParsing = true;
        m_willBeParserExecuted = true;
    } else if (client->hasSourceAttribute() && m_parserInserted && !client->asyncAttributeValue()) {
        m_willBeParserExecuted = true;
    } else if (!client->hasSourceAttribute() && m_parserInserted && !elementDocument.haveStylesheetsAndImportsLoaded()) {
        m_willBeParserExecuted = true;
        m_readyToBeParserExecuted = true;
    } else if (client->hasSourceAttribute() && !client->asyncAttributeValue() && !m_forceAsync) {
        m_willExecuteInOrder = true;
        contextDocument->scriptRunner()->queueScriptForExecution(this, m_resource, ScriptRunner::IN_ORDER_EXECUTION);
        m_resource->addClient(this);
    } else if (client->hasSourceAttribute()) {
        contextDocument->scriptRunner()->queueScriptForExecution(this, m_resource, ScriptRunner::ASYNC_EXECUTION);
        m_resource->addClient(this);
    } else {
        TextPosition position = elementDocument.isInDocumentWrite() ? TextPosition() : scriptStartPosition;
        KURL scriptURL = (!elementDocument.isInDocumentWrite() && m_parserInserted) ? elementDocument.url() : KURL();
        executeScript(ScriptSourceCode(scriptContent(), scriptURL, position));
    }

    return true;
}
