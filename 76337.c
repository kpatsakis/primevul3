void InlineTextBox::paintDocumentMarkers(GraphicsContext* pt, int tx, int ty, RenderStyle* style, const Font& font, bool background)
{
    if (!renderer()->node())
        return;

    Vector<DocumentMarker> markers = renderer()->document()->markers()->markersForNode(renderer()->node());
    Vector<DocumentMarker>::iterator markerIt = markers.begin();

    for ( ; markerIt != markers.end(); markerIt++) {
        const DocumentMarker& marker = *markerIt;
        
        switch (marker.type) {
            case DocumentMarker::Grammar:
            case DocumentMarker::Spelling:
            case DocumentMarker::Replacement:
            case DocumentMarker::CorrectionIndicator:
            case DocumentMarker::RejectedCorrection:
                if (background)
                    continue;
                break;
            case DocumentMarker::TextMatch:
                if (!background)
                    continue;
                break;
            
            default:
                ASSERT_NOT_REACHED();
        }

        if (marker.endOffset <= start())
            continue;
        
        if (marker.startOffset > end())
            break;
        
        switch (marker.type) {
            case DocumentMarker::Spelling:
                paintSpellingOrGrammarMarker(pt, tx, ty, marker, style, font, false);
                break;
            case DocumentMarker::Grammar:
                paintSpellingOrGrammarMarker(pt, tx, ty, marker, style, font, true);
                break;
            case DocumentMarker::TextMatch:
                paintTextMatchMarker(pt, tx, ty, marker, style, font);
                break;
            case DocumentMarker::CorrectionIndicator:
                computeRectForReplacementMarker(tx, ty, marker, style, font);
                paintSpellingOrGrammarMarker(pt, tx, ty, marker, style, font, false);
                break;
            case DocumentMarker::Replacement:
            case DocumentMarker::RejectedCorrection:
                break;
            default:
                ASSERT_NOT_REACHED();
        }

    }
}
