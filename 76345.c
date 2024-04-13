static GraphicsContext::TextCheckingLineStyle textCheckingLineStyleForMarkerType(DocumentMarker::MarkerType markerType)
{
    switch (markerType) {
    case DocumentMarker::Spelling:
        return GraphicsContext::TextCheckingSpellingLineStyle;
    case DocumentMarker::Grammar:
        return GraphicsContext::TextCheckingGrammarLineStyle;
    case DocumentMarker::CorrectionIndicator:
        return GraphicsContext::TextCheckingReplacementLineStyle;
    default:
        ASSERT_NOT_REACHED();
        return GraphicsContext::TextCheckingSpellingLineStyle;
    }
}
