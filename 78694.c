void FrameLoader::setReferrerForFrameRequest(ResourceRequest& request, ShouldSendReferrer shouldSendReferrer)
{
    if (shouldSendReferrer == NeverSendReferrer) {
        request.clearHTTPReferrer();
        return;
    }

    String argsReferrer(request.httpReferrer());
    if (argsReferrer.isEmpty())
        argsReferrer = outgoingReferrer();
    String referrer = SecurityPolicy::generateReferrerHeader(m_frame->document()->referrerPolicy(), request.url(), argsReferrer);

    request.setHTTPReferrer(referrer);
    RefPtr<SecurityOrigin> referrerOrigin = SecurityOrigin::createFromString(referrer);
    addHTTPOriginIfNeeded(request, referrerOrigin->toString());
}
