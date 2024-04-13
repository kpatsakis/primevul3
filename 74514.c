static bool canAccessAncestor(const SecurityOrigin* activeSecurityOrigin, Frame* targetFrame)
{
    if (!targetFrame)
        return false;

    for (Frame* ancestorFrame = targetFrame; ancestorFrame; ancestorFrame = ancestorFrame->tree()->parent()) {
        Document* ancestorDocument = ancestorFrame->document();
        if (!ancestorDocument)
            return true;

        const SecurityOrigin* ancestorSecurityOrigin = ancestorDocument->securityOrigin();
        if (activeSecurityOrigin->canAccess(ancestorSecurityOrigin))
            return true;
    }

    return false;
}
