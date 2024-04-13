PassRefPtr<CSSValue> CSSComputedStyleDeclaration::getPropertyCSSValue(CSSPropertyID propertyID, EUpdateLayout updateLayout) const
{
    Node* node = m_node.get();
    if (!node)
        return 0;

    if (updateLayout)
        node->document()->updateLayoutIgnorePendingStylesheets();

    RenderObject* renderer = node->renderer();

    RefPtr<RenderStyle> style;
    if (renderer && renderer->isComposited() && AnimationController::supportsAcceleratedAnimationOfProperty(propertyID)) {
        AnimationUpdateBlock animationUpdateBlock(renderer->animation());
        style = renderer->animation()->getAnimatedStyleForRenderer(renderer);
        if (m_pseudoElementSpecifier) {
            style = style->getCachedPseudoStyle(m_pseudoElementSpecifier);
        }
    } else
        style = node->computedStyle(m_pseudoElementSpecifier);

    if (!style)
        return 0;

    if (renderer) {
        if (m_pseudoElementSpecifier == AFTER)
            renderer = renderer->afterPseudoElementRenderer();
        else if (m_pseudoElementSpecifier == BEFORE)
            renderer = renderer->beforePseudoElementRenderer();
    }

    propertyID = CSSProperty::resolveDirectionAwareProperty(propertyID, style->direction(), style->writingMode());

    switch (propertyID) {
        case CSSPropertyInvalid:
#if ENABLE(CSS_VARIABLES)
        case CSSPropertyVariable:
#endif
            break;

        case CSSPropertyBackgroundColor:
            return cssValuePool().createColorValue(m_allowVisitedStyle? style->visitedDependentColor(CSSPropertyBackgroundColor).rgb() : style->backgroundColor().rgb());
        case CSSPropertyBackgroundImage:
        case CSSPropertyWebkitMaskImage: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskImage ? style->maskLayers() : style->backgroundLayers();
            if (!layers)
                return cssValuePool().createIdentifierValue(CSSValueNone);

            if (!layers->next()) {
                if (layers->image())
                    return layers->image()->cssValue();

                return cssValuePool().createIdentifierValue(CSSValueNone);
            }

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next()) {
                if (currLayer->image())
                    list->append(currLayer->image()->cssValue());
                else
                    list->append(cssValuePool().createIdentifierValue(CSSValueNone));
            }
            return list.release();
        }
        case CSSPropertyBackgroundSize:
        case CSSPropertyWebkitBackgroundSize:
        case CSSPropertyWebkitMaskSize: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskSize ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return fillSizeToCSSValue(layers->size(), style.get());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(fillSizeToCSSValue(currLayer->size(), style.get()));

            return list.release();
        }
        case CSSPropertyBackgroundRepeat:
        case CSSPropertyWebkitMaskRepeat: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskRepeat ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return fillRepeatToCSSValue(layers->repeatX(), layers->repeatY());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(fillRepeatToCSSValue(currLayer->repeatX(), currLayer->repeatY()));

            return list.release();
        }
        case CSSPropertyWebkitBackgroundComposite:
        case CSSPropertyWebkitMaskComposite: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskComposite ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return cssValuePool().createValue(layers->composite());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(cssValuePool().createValue(currLayer->composite()));

            return list.release();
        }
        case CSSPropertyBackgroundAttachment:
        case CSSPropertyWebkitMaskAttachment: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskAttachment ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return cssValuePool().createValue(layers->attachment());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(cssValuePool().createValue(currLayer->attachment()));

            return list.release();
        }
        case CSSPropertyBackgroundClip:
        case CSSPropertyBackgroundOrigin:
        case CSSPropertyWebkitBackgroundClip:
        case CSSPropertyWebkitBackgroundOrigin:
        case CSSPropertyWebkitMaskClip:
        case CSSPropertyWebkitMaskOrigin: {
            const FillLayer* layers = (propertyID == CSSPropertyWebkitMaskClip || propertyID == CSSPropertyWebkitMaskOrigin) ? style->maskLayers() : style->backgroundLayers();
            bool isClip = propertyID == CSSPropertyBackgroundClip || propertyID == CSSPropertyWebkitBackgroundClip || propertyID == CSSPropertyWebkitMaskClip;
            if (!layers->next()) {
                EFillBox box = isClip ? layers->clip() : layers->origin();
                return cssValuePool().createValue(box);
            }

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next()) {
                EFillBox box = isClip ? currLayer->clip() : currLayer->origin();
                list->append(cssValuePool().createValue(box));
            }

            return list.release();
        }
        case CSSPropertyBackgroundPosition:
        case CSSPropertyWebkitMaskPosition: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskPosition ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next()) {
                RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
                list->append(zoomAdjustedPixelValueForLength(layers->xPosition(), style.get()));
                list->append(zoomAdjustedPixelValueForLength(layers->yPosition(), style.get()));
                return list.release();
            }

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next()) {
                RefPtr<CSSValueList> positionList = CSSValueList::createSpaceSeparated();
                positionList->append(zoomAdjustedPixelValueForLength(currLayer->xPosition(), style.get()));
                positionList->append(zoomAdjustedPixelValueForLength(currLayer->yPosition(), style.get()));
                list->append(positionList);
            }

            return list.release();
        }
        case CSSPropertyBackgroundPositionX:
        case CSSPropertyWebkitMaskPositionX: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskPositionX ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return cssValuePool().createValue(layers->xPosition());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(cssValuePool().createValue(currLayer->xPosition()));

            return list.release();
        }
        case CSSPropertyBackgroundPositionY:
        case CSSPropertyWebkitMaskPositionY: {
            const FillLayer* layers = propertyID == CSSPropertyWebkitMaskPositionY ? style->maskLayers() : style->backgroundLayers();
            if (!layers->next())
                return cssValuePool().createValue(layers->yPosition());

            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (const FillLayer* currLayer = layers; currLayer; currLayer = currLayer->next())
                list->append(cssValuePool().createValue(currLayer->yPosition()));

            return list.release();
        }
        case CSSPropertyBorderCollapse:
            if (style->borderCollapse())
                return cssValuePool().createIdentifierValue(CSSValueCollapse);
            return cssValuePool().createIdentifierValue(CSSValueSeparate);
        case CSSPropertyBorderSpacing: {
            RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
            list->append(zoomAdjustedPixelValue(style->horizontalBorderSpacing(), style.get()));
            list->append(zoomAdjustedPixelValue(style->verticalBorderSpacing(), style.get()));
            return list.release();
        }
        case CSSPropertyWebkitBorderHorizontalSpacing:
            return zoomAdjustedPixelValue(style->horizontalBorderSpacing(), style.get());
        case CSSPropertyWebkitBorderVerticalSpacing:
            return zoomAdjustedPixelValue(style->verticalBorderSpacing(), style.get());
        case CSSPropertyBorderImageSource:
            if (style->borderImageSource())
                return style->borderImageSource()->cssValue();
            return cssValuePool().createIdentifierValue(CSSValueNone);
        case CSSPropertyBorderTopColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyBorderTopColor).rgb()) : currentColorOrValidColor(style.get(), style->borderTopColor());
        case CSSPropertyBorderRightColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyBorderRightColor).rgb()) : currentColorOrValidColor(style.get(), style->borderRightColor());
        case CSSPropertyBorderBottomColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyBorderBottomColor).rgb()) : currentColorOrValidColor(style.get(), style->borderBottomColor());
        case CSSPropertyBorderLeftColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyBorderLeftColor).rgb()) : currentColorOrValidColor(style.get(), style->borderLeftColor());
        case CSSPropertyBorderTopStyle:
            return cssValuePool().createValue(style->borderTopStyle());
        case CSSPropertyBorderRightStyle:
            return cssValuePool().createValue(style->borderRightStyle());
        case CSSPropertyBorderBottomStyle:
            return cssValuePool().createValue(style->borderBottomStyle());
        case CSSPropertyBorderLeftStyle:
            return cssValuePool().createValue(style->borderLeftStyle());
        case CSSPropertyBorderTopWidth:
            return zoomAdjustedPixelValue(style->borderTopWidth(), style.get());
        case CSSPropertyBorderRightWidth:
            return zoomAdjustedPixelValue(style->borderRightWidth(), style.get());
        case CSSPropertyBorderBottomWidth:
            return zoomAdjustedPixelValue(style->borderBottomWidth(), style.get());
        case CSSPropertyBorderLeftWidth:
            return zoomAdjustedPixelValue(style->borderLeftWidth(), style.get());
        case CSSPropertyBottom:
            return getPositionOffsetValue(style.get(), CSSPropertyBottom, m_node->document()->renderView());
        case CSSPropertyWebkitBoxAlign:
            return cssValuePool().createValue(style->boxAlign());
#if ENABLE(CSS_BOX_DECORATION_BREAK)
        case CSSPropertyWebkitBoxDecorationBreak:
            if (style->boxDecorationBreak() == DSLICE)
                return cssValuePool().createIdentifierValue(CSSValueSlice);
        return cssValuePool().createIdentifierValue(CSSValueClone);
#endif
        case CSSPropertyWebkitBoxDirection:
            return cssValuePool().createValue(style->boxDirection());
        case CSSPropertyWebkitBoxFlex:
            return cssValuePool().createValue(style->boxFlex(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitBoxFlexGroup:
            return cssValuePool().createValue(style->boxFlexGroup(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitBoxLines:
            return cssValuePool().createValue(style->boxLines());
        case CSSPropertyWebkitBoxOrdinalGroup:
            return cssValuePool().createValue(style->boxOrdinalGroup(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitBoxOrient:
            return cssValuePool().createValue(style->boxOrient());
        case CSSPropertyWebkitBoxPack:
            return cssValuePool().createValue(style->boxPack());
        case CSSPropertyWebkitBoxReflect:
            return valueForReflection(style->boxReflect(), style.get());
        case CSSPropertyBoxShadow:
        case CSSPropertyWebkitBoxShadow:
            return valueForShadow(style->boxShadow(), propertyID, style.get());
        case CSSPropertyCaptionSide:
            return cssValuePool().createValue(style->captionSide());
        case CSSPropertyClear:
            return cssValuePool().createValue(style->clear());
        case CSSPropertyColor:
            return cssValuePool().createColorValue(m_allowVisitedStyle ? style->visitedDependentColor(CSSPropertyColor).rgb() : style->color().rgb());
        case CSSPropertyWebkitPrintColorAdjust:
            return cssValuePool().createValue(style->printColorAdjust());
        case CSSPropertyWebkitColumnAxis:
            return cssValuePool().createValue(style->columnAxis());
        case CSSPropertyWebkitColumnCount:
            if (style->hasAutoColumnCount())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->columnCount(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitColumnGap:
            if (style->hasNormalColumnGap())
                return cssValuePool().createIdentifierValue(CSSValueNormal);
            return zoomAdjustedPixelValue(style->columnGap(), style.get());
        case CSSPropertyWebkitColumnProgression:
            return cssValuePool().createValue(style->columnProgression());
        case CSSPropertyWebkitColumnRuleColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyOutlineColor).rgb()) : currentColorOrValidColor(style.get(), style->columnRuleColor());
        case CSSPropertyWebkitColumnRuleStyle:
            return cssValuePool().createValue(style->columnRuleStyle());
        case CSSPropertyWebkitColumnRuleWidth:
            return zoomAdjustedPixelValue(style->columnRuleWidth(), style.get());
        case CSSPropertyWebkitColumnSpan:
            if (style->columnSpan())
                return cssValuePool().createIdentifierValue(CSSValueAll);
            return cssValuePool().createValue(1, CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitColumnBreakAfter:
            return cssValuePool().createValue(style->columnBreakAfter());
        case CSSPropertyWebkitColumnBreakBefore:
            return cssValuePool().createValue(style->columnBreakBefore());
        case CSSPropertyWebkitColumnBreakInside:
            return cssValuePool().createValue(style->columnBreakInside());
        case CSSPropertyWebkitColumnWidth:
            if (style->hasAutoColumnWidth())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return zoomAdjustedPixelValue(style->columnWidth(), style.get());
        case CSSPropertyTabSize:
            return cssValuePool().createValue(style->tabSize(), CSSPrimitiveValue::CSS_NUMBER);
#if ENABLE(CSS_REGIONS)
        case CSSPropertyWebkitRegionBreakAfter:
            return cssValuePool().createValue(style->regionBreakAfter());
        case CSSPropertyWebkitRegionBreakBefore:
            return cssValuePool().createValue(style->regionBreakBefore());
        case CSSPropertyWebkitRegionBreakInside:
            return cssValuePool().createValue(style->regionBreakInside());
#endif
        case CSSPropertyCursor: {
            RefPtr<CSSValueList> list;
            CursorList* cursors = style->cursors();
            if (cursors && cursors->size() > 0) {
                list = CSSValueList::createCommaSeparated();
                for (unsigned i = 0; i < cursors->size(); ++i)
                    if (StyleImage* image = cursors->at(i).image())
                        list->append(image->cssValue());
            }
            RefPtr<CSSValue> value = cssValuePool().createValue(style->cursor());
            if (list) {
                list->append(value);
                return list.release();
            }
            return value.release();
        }
        case CSSPropertyDirection:
            return cssValuePool().createValue(style->direction());
        case CSSPropertyDisplay:
            return cssValuePool().createValue(style->display());
        case CSSPropertyEmptyCells:
            return cssValuePool().createValue(style->emptyCells());
#if ENABLE(CSS3_FLEXBOX)
        case CSSPropertyWebkitFlex: {
            RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
            list->append(cssValuePool().createValue(style->flexGrow()));
            list->append(cssValuePool().createValue(style->flexShrink()));

            Length flexBasis = style->flexBasis();
            if (flexBasis.isAuto())
                list->append(cssValuePool().createIdentifierValue(CSSValueAuto));
            else if (flexBasis.isPercent())
                list->append(cssValuePool().createValue(flexBasis.value(), CSSPrimitiveValue::CSS_PERCENTAGE));
            else
                list->append(cssValuePool().createValue(flexBasis.value(), CSSPrimitiveValue::CSS_PX));

            return list.release();
        }
        case CSSPropertyWebkitOrder:
            return cssValuePool().createValue(style->order());
        case CSSPropertyWebkitJustifyContent:
            return cssValuePool().createValue(style->justifyContent());
        case CSSPropertyWebkitAlignItems:
            return cssValuePool().createValue(style->alignItems());
        case CSSPropertyWebkitAlignSelf:
            if (style->alignSelf() == AlignAuto) {
                if (m_node && m_node->parentNode() && m_node->parentNode()->computedStyle())
                    return cssValuePool().createValue(m_node->parentNode()->computedStyle()->alignItems());
                return cssValuePool().createValue(AlignStretch);
            }
            return cssValuePool().createValue(style->alignSelf());
        case CSSPropertyWebkitFlexDirection:
            return cssValuePool().createValue(style->flexDirection());
        case CSSPropertyWebkitFlexWrap:
            return cssValuePool().createValue(style->flexWrap());
        case CSSPropertyWebkitAlignContent:
            return cssValuePool().createValue(style->alignContent());
        case CSSPropertyWebkitFlexFlow: {
            RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
            list->append(cssValuePool().createValue(style->flexDirection()));
            list->append(cssValuePool().createValue(style->flexWrap()));
            return list.release();
        }
#endif
        case CSSPropertyFloat:
            return cssValuePool().createValue(style->floating());
        case CSSPropertyFont: {
            RefPtr<FontValue> computedFont = FontValue::create();
            computedFont->style = fontStyleFromStyle(style.get());
            computedFont->variant = fontVariantFromStyle(style.get());
            computedFont->weight = fontWeightFromStyle(style.get());
            computedFont->size = fontSizeFromStyle(style.get());
            computedFont->lineHeight = lineHeightFromStyle(style.get(), m_node->document()->renderView());
            computedFont->family = fontFamilyFromStyle(style.get());
            return computedFont.release();
        }
        case CSSPropertyFontFamily: {
            RefPtr<CSSValueList> fontFamilyList = fontFamilyFromStyle(style.get());
            if (fontFamilyList->length() == 1)
                return fontFamilyList->item(0);
            return fontFamilyList.release();
        }
        case CSSPropertyFontSize:
            return fontSizeFromStyle(style.get());
        case CSSPropertyFontStyle:
            return fontStyleFromStyle(style.get());
        case CSSPropertyFontVariant:
            return fontVariantFromStyle(style.get());
        case CSSPropertyFontWeight:
            return fontWeightFromStyle(style.get());
        case CSSPropertyWebkitFontFeatureSettings: {
            const FontFeatureSettings* featureSettings = style->fontDescription().featureSettings();
            if (!featureSettings || !featureSettings->size())
                return cssValuePool().createIdentifierValue(CSSValueNormal);
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            for (unsigned i = 0; i < featureSettings->size(); ++i) {
                const FontFeature& feature = featureSettings->at(i);
                RefPtr<FontFeatureValue> featureValue = FontFeatureValue::create(feature.tag(), feature.value());
                list->append(featureValue.release());
            }
            return list.release();
        }
        case CSSPropertyWebkitGridColumns: {
            return valueForGridTrackList(style->gridColumns(), style.get());
        }
        case CSSPropertyWebkitGridRows: {
            return valueForGridTrackList(style->gridRows(), style.get());
        }

        case CSSPropertyWebkitGridColumn:
            return valueForGridPosition(style->gridItemColumn());
        case CSSPropertyWebkitGridRow:
            return valueForGridPosition(style->gridItemRow());

        case CSSPropertyHeight:
            if (renderer) {
                if (!renderer->isReplaced() && renderer->isInline())
                    return cssValuePool().createIdentifierValue(CSSValueAuto);
                return zoomAdjustedPixelValue(sizingBox(renderer).height(), style.get());
            }
            return zoomAdjustedPixelValueForLength(style->height(), style.get());
        case CSSPropertyWebkitHighlight:
            if (style->highlight() == nullAtom)
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return cssValuePool().createValue(style->highlight(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyWebkitHyphens:
            return cssValuePool().createValue(style->hyphens());
        case CSSPropertyWebkitHyphenateCharacter:
            if (style->hyphenationString().isNull())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->hyphenationString(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyWebkitHyphenateLimitAfter:
            if (style->hyphenationLimitAfter() < 0)
                return CSSPrimitiveValue::createIdentifier(CSSValueAuto);
            return CSSPrimitiveValue::create(style->hyphenationLimitAfter(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitHyphenateLimitBefore:
            if (style->hyphenationLimitBefore() < 0)
                return CSSPrimitiveValue::createIdentifier(CSSValueAuto);
            return CSSPrimitiveValue::create(style->hyphenationLimitBefore(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitHyphenateLimitLines:
            if (style->hyphenationLimitLines() < 0)
                return CSSPrimitiveValue::createIdentifier(CSSValueNoLimit);
            return CSSPrimitiveValue::create(style->hyphenationLimitLines(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitBorderFit:
            if (style->borderFit() == BorderFitBorder)
                return cssValuePool().createIdentifierValue(CSSValueBorder);
            return cssValuePool().createIdentifierValue(CSSValueLines);
        case CSSPropertyImageRendering:
            return CSSPrimitiveValue::create(style->imageRendering());
#if ENABLE(CSS_IMAGE_RESOLUTION)
        case CSSPropertyImageResolution:
            return cssValuePool().createValue(style->imageResolution(), CSSPrimitiveValue::CSS_DPPX);
#endif
        case CSSPropertyLeft:
            return getPositionOffsetValue(style.get(), CSSPropertyLeft, m_node->document()->renderView());
        case CSSPropertyLetterSpacing:
            if (!style->letterSpacing())
                return cssValuePool().createIdentifierValue(CSSValueNormal);
            return zoomAdjustedPixelValue(style->letterSpacing(), style.get());
        case CSSPropertyWebkitLineClamp:
            if (style->lineClamp().isNone())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return cssValuePool().createValue(style->lineClamp().value(), style->lineClamp().isPercentage() ? CSSPrimitiveValue::CSS_PERCENTAGE : CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyLineHeight:
            return lineHeightFromStyle(style.get(), m_node->document()->renderView());
        case CSSPropertyListStyleImage:
            if (style->listStyleImage())
                return style->listStyleImage()->cssValue();
            return cssValuePool().createIdentifierValue(CSSValueNone);
        case CSSPropertyListStylePosition:
            return cssValuePool().createValue(style->listStylePosition());
        case CSSPropertyListStyleType:
            return cssValuePool().createValue(style->listStyleType());
        case CSSPropertyWebkitLocale:
            if (style->locale().isNull())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->locale(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyMarginTop: {
            Length marginTop = style->marginTop();
            if (marginTop.isFixed() || !renderer || !renderer->isBox())
                return zoomAdjustedPixelValueForLength(marginTop, style.get());
            return zoomAdjustedPixelValue(toRenderBox(renderer)->marginTop(), style.get());
        }
        case CSSPropertyMarginRight: {
            Length marginRight = style->marginRight();
            if (marginRight.isFixed() || !renderer || !renderer->isBox())
                return zoomAdjustedPixelValueForLength(marginRight, style.get());
            int value;
            if (marginRight.isPercent() || marginRight.isViewportPercentage())
                value = minimumValueForLength(marginRight, toRenderBox(renderer)->containingBlockLogicalWidthForContent(), m_node->document()->renderView());
            else
                value = toRenderBox(renderer)->marginRight();
            return zoomAdjustedPixelValue(value, style.get());
        }
        case CSSPropertyMarginBottom: {
            Length marginBottom = style->marginBottom();
            if (marginBottom.isFixed() || !renderer || !renderer->isBox())
                return zoomAdjustedPixelValueForLength(marginBottom, style.get());
            return zoomAdjustedPixelValue(toRenderBox(renderer)->marginBottom(), style.get());
        }
        case CSSPropertyMarginLeft: {
            Length marginLeft = style->marginLeft();
            if (marginLeft.isFixed() || !renderer || !renderer->isBox())
                return zoomAdjustedPixelValueForLength(marginLeft, style.get());
            return zoomAdjustedPixelValue(toRenderBox(renderer)->marginLeft(), style.get());
        }
        case CSSPropertyWebkitMarqueeDirection:
            return cssValuePool().createValue(style->marqueeDirection());
        case CSSPropertyWebkitMarqueeIncrement:
            return cssValuePool().createValue(style->marqueeIncrement());
        case CSSPropertyWebkitMarqueeRepetition:
            if (style->marqueeLoopCount() < 0)
                return cssValuePool().createIdentifierValue(CSSValueInfinite);
            return cssValuePool().createValue(style->marqueeLoopCount(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWebkitMarqueeStyle:
            return cssValuePool().createValue(style->marqueeBehavior());
        case CSSPropertyWebkitUserModify:
            return cssValuePool().createValue(style->userModify());
        case CSSPropertyMaxHeight: {
            const Length& maxHeight = style->maxHeight();
            if (maxHeight.isUndefined())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return zoomAdjustedPixelValueForLength(maxHeight, style.get());
        }
        case CSSPropertyMaxWidth: {
            const Length& maxWidth = style->maxWidth();
            if (maxWidth.isUndefined())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return zoomAdjustedPixelValueForLength(maxWidth, style.get());
        }
        case CSSPropertyMinHeight:
            return zoomAdjustedPixelValueForLength(style->minHeight(), style.get());
        case CSSPropertyMinWidth:
            return zoomAdjustedPixelValueForLength(style->minWidth(), style.get());
        case CSSPropertyOpacity:
            return cssValuePool().createValue(style->opacity(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyOrphans:
            return cssValuePool().createValue(style->orphans(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyOutlineColor:
            return m_allowVisitedStyle ? cssValuePool().createColorValue(style->visitedDependentColor(CSSPropertyOutlineColor).rgb()) : currentColorOrValidColor(style.get(), style->outlineColor());
        case CSSPropertyOutlineOffset:
            return zoomAdjustedPixelValue(style->outlineOffset(), style.get());
        case CSSPropertyOutlineStyle:
            if (style->outlineStyleIsAuto())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->outlineStyle());
        case CSSPropertyOutlineWidth:
            return zoomAdjustedPixelValue(style->outlineWidth(), style.get());
        case CSSPropertyOverflow:
            return cssValuePool().createValue(max(style->overflowX(), style->overflowY()));
        case CSSPropertyOverflowX:
            return cssValuePool().createValue(style->overflowX());
        case CSSPropertyOverflowY:
            return cssValuePool().createValue(style->overflowY());
        case CSSPropertyPaddingTop:
            if (renderer && renderer->isBox())
                return zoomAdjustedPixelValue(toRenderBox(renderer)->computedCSSPaddingTop(), style.get());
            return zoomAdjustedPixelValueForLength(style->paddingTop(), style.get());
        case CSSPropertyPaddingRight:
            if (renderer && renderer->isBox())
                return zoomAdjustedPixelValue(toRenderBox(renderer)->computedCSSPaddingRight(), style.get());
            return zoomAdjustedPixelValueForLength(style->paddingRight(), style.get());
        case CSSPropertyPaddingBottom:
            if (renderer && renderer->isBox())
                return zoomAdjustedPixelValue(toRenderBox(renderer)->computedCSSPaddingBottom(), style.get());
            return zoomAdjustedPixelValueForLength(style->paddingBottom(), style.get());
        case CSSPropertyPaddingLeft:
            if (renderer && renderer->isBox())
                return zoomAdjustedPixelValue(toRenderBox(renderer)->computedCSSPaddingLeft(), style.get());
            return zoomAdjustedPixelValueForLength(style->paddingLeft(), style.get());
        case CSSPropertyPageBreakAfter:
            return cssValuePool().createValue(style->pageBreakAfter());
        case CSSPropertyPageBreakBefore:
            return cssValuePool().createValue(style->pageBreakBefore());
        case CSSPropertyPageBreakInside: {
            EPageBreak pageBreak = style->pageBreakInside();
            ASSERT(pageBreak != PBALWAYS);
            if (pageBreak == PBALWAYS)
                return 0;
            return cssValuePool().createValue(style->pageBreakInside());
        }
        case CSSPropertyPosition:
            return cssValuePool().createValue(style->position());
        case CSSPropertyRight:
            return getPositionOffsetValue(style.get(), CSSPropertyRight, m_node->document()->renderView());
        case CSSPropertyTableLayout:
            return cssValuePool().createValue(style->tableLayout());
        case CSSPropertyTextAlign:
            return cssValuePool().createValue(style->textAlign());
        case CSSPropertyTextDecoration:
            return renderTextDecorationFlagsToCSSValue(style->textDecoration());
        case CSSPropertyWebkitTextDecorationsInEffect:
            return renderTextDecorationFlagsToCSSValue(style->textDecorationsInEffect());
        case CSSPropertyWebkitTextFillColor:
            return currentColorOrValidColor(style.get(), style->textFillColor());
        case CSSPropertyWebkitTextEmphasisColor:
            return currentColorOrValidColor(style.get(), style->textEmphasisColor());
        case CSSPropertyWebkitTextEmphasisPosition:
            return cssValuePool().createValue(style->textEmphasisPosition());
        case CSSPropertyWebkitTextEmphasisStyle:
            switch (style->textEmphasisMark()) {
            case TextEmphasisMarkNone:
                return cssValuePool().createIdentifierValue(CSSValueNone);
            case TextEmphasisMarkCustom:
                return cssValuePool().createValue(style->textEmphasisCustomMark(), CSSPrimitiveValue::CSS_STRING);
            case TextEmphasisMarkAuto:
                ASSERT_NOT_REACHED();
            case TextEmphasisMarkDot:
            case TextEmphasisMarkCircle:
            case TextEmphasisMarkDoubleCircle:
            case TextEmphasisMarkTriangle:
            case TextEmphasisMarkSesame: {
                RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
                list->append(cssValuePool().createValue(style->textEmphasisFill()));
                list->append(cssValuePool().createValue(style->textEmphasisMark()));
                return list.release();
            }
            }
        case CSSPropertyTextIndent:
            return zoomAdjustedPixelValueForLength(style->textIndent(), style.get());
        case CSSPropertyTextShadow:
            return valueForShadow(style->textShadow(), propertyID, style.get());
        case CSSPropertyTextRendering:
            return cssValuePool().createValue(style->fontDescription().textRenderingMode());
        case CSSPropertyTextOverflow:
            if (style->textOverflow())
                return cssValuePool().createIdentifierValue(CSSValueEllipsis);
            return cssValuePool().createIdentifierValue(CSSValueClip);
        case CSSPropertyWebkitTextSecurity:
            return cssValuePool().createValue(style->textSecurity());
        case CSSPropertyWebkitTextSizeAdjust:
            if (style->textSizeAdjust())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createIdentifierValue(CSSValueNone);
        case CSSPropertyWebkitTextStrokeColor:
            return currentColorOrValidColor(style.get(), style->textStrokeColor());
        case CSSPropertyWebkitTextStrokeWidth:
            return zoomAdjustedPixelValue(style->textStrokeWidth(), style.get());
        case CSSPropertyTextTransform:
            return cssValuePool().createValue(style->textTransform());
        case CSSPropertyTop:
            return getPositionOffsetValue(style.get(), CSSPropertyTop, m_node->document()->renderView());
        case CSSPropertyUnicodeBidi:
            return renderUnicodeBidiFlagsToCSSValue(style->unicodeBidi());
        case CSSPropertyVerticalAlign:
            switch (style->verticalAlign()) {
                case BASELINE:
                    return cssValuePool().createIdentifierValue(CSSValueBaseline);
                case MIDDLE:
                    return cssValuePool().createIdentifierValue(CSSValueMiddle);
                case SUB:
                    return cssValuePool().createIdentifierValue(CSSValueSub);
                case SUPER:
                    return cssValuePool().createIdentifierValue(CSSValueSuper);
                case TEXT_TOP:
                    return cssValuePool().createIdentifierValue(CSSValueTextTop);
                case TEXT_BOTTOM:
                    return cssValuePool().createIdentifierValue(CSSValueTextBottom);
                case TOP:
                    return cssValuePool().createIdentifierValue(CSSValueTop);
                case BOTTOM:
                    return cssValuePool().createIdentifierValue(CSSValueBottom);
                case BASELINE_MIDDLE:
                    return cssValuePool().createIdentifierValue(CSSValueWebkitBaselineMiddle);
                case LENGTH:
                    return cssValuePool().createValue(style->verticalAlignLength());
            }
            ASSERT_NOT_REACHED();
            return 0;
        case CSSPropertyVisibility:
            return cssValuePool().createValue(style->visibility());
        case CSSPropertyWhiteSpace:
            return cssValuePool().createValue(style->whiteSpace());
        case CSSPropertyWidows:
            return cssValuePool().createValue(style->widows(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyWidth:
            if (renderer) {
                if (!renderer->isReplaced() && renderer->isInline())
                    return cssValuePool().createIdentifierValue(CSSValueAuto);
                return zoomAdjustedPixelValue(sizingBox(renderer).width(), style.get());
            }
            return zoomAdjustedPixelValueForLength(style->width(), style.get());
        case CSSPropertyWordBreak:
            return cssValuePool().createValue(style->wordBreak());
        case CSSPropertyWordSpacing:
            return zoomAdjustedPixelValue(style->wordSpacing(), style.get());
        case CSSPropertyWordWrap:
            return cssValuePool().createValue(style->wordWrap());
        case CSSPropertyWebkitLineBreak:
            return cssValuePool().createValue(style->khtmlLineBreak());
        case CSSPropertyWebkitNbspMode:
            return cssValuePool().createValue(style->nbspMode());
        case CSSPropertyResize:
            return cssValuePool().createValue(style->resize());
        case CSSPropertyWebkitFontKerning:
            return cssValuePool().createValue(style->fontDescription().kerning());
        case CSSPropertyWebkitFontSmoothing:
            return cssValuePool().createValue(style->fontDescription().fontSmoothing());
        case CSSPropertyWebkitFontVariantLigatures: {
            FontDescription::LigaturesState commonLigaturesState = style->fontDescription().commonLigaturesState();
            FontDescription::LigaturesState discretionaryLigaturesState = style->fontDescription().discretionaryLigaturesState();
            FontDescription::LigaturesState historicalLigaturesState = style->fontDescription().historicalLigaturesState();
            if (commonLigaturesState == FontDescription::NormalLigaturesState && discretionaryLigaturesState == FontDescription::NormalLigaturesState
                && historicalLigaturesState == FontDescription::NormalLigaturesState)
                return cssValuePool().createIdentifierValue(CSSValueNormal);

            RefPtr<CSSValueList> valueList = CSSValueList::createSpaceSeparated();
            if (commonLigaturesState != FontDescription::NormalLigaturesState)
                valueList->append(cssValuePool().createIdentifierValue(commonLigaturesState == FontDescription::DisabledLigaturesState ? CSSValueNoCommonLigatures : CSSValueCommonLigatures));
            if (discretionaryLigaturesState != FontDescription::NormalLigaturesState)
                valueList->append(cssValuePool().createIdentifierValue(discretionaryLigaturesState == FontDescription::DisabledLigaturesState ? CSSValueNoDiscretionaryLigatures : CSSValueDiscretionaryLigatures));
            if (historicalLigaturesState != FontDescription::NormalLigaturesState)
                valueList->append(cssValuePool().createIdentifierValue(historicalLigaturesState == FontDescription::DisabledLigaturesState ? CSSValueNoHistoricalLigatures : CSSValueHistoricalLigatures));
            return valueList;
        }
        case CSSPropertyZIndex:
            if (style->hasAutoZIndex())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->zIndex(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyZoom:
            return cssValuePool().createValue(style->zoom(), CSSPrimitiveValue::CSS_NUMBER);
        case CSSPropertyBoxSizing:
            if (style->boxSizing() == CONTENT_BOX)
                return cssValuePool().createIdentifierValue(CSSValueContentBox);
            return cssValuePool().createIdentifierValue(CSSValueBorderBox);
#if ENABLE(DASHBOARD_SUPPORT)
        case CSSPropertyWebkitDashboardRegion:
        {
            const Vector<StyleDashboardRegion>& regions = style->dashboardRegions();
            unsigned count = regions.size();
            if (count == 1 && regions[0].type == StyleDashboardRegion::None)
                return cssValuePool().createIdentifierValue(CSSValueNone);

            RefPtr<DashboardRegion> firstRegion;
            DashboardRegion* previousRegion = 0;
            for (unsigned i = 0; i < count; i++) {
                RefPtr<DashboardRegion> region = DashboardRegion::create();
                StyleDashboardRegion styleRegion = regions[i];

                region->m_label = styleRegion.label;
                LengthBox offset = styleRegion.offset;
                region->setTop(zoomAdjustedPixelValue(offset.top().value(), style.get()));
                region->setRight(zoomAdjustedPixelValue(offset.right().value(), style.get()));
                region->setBottom(zoomAdjustedPixelValue(offset.bottom().value(), style.get()));
                region->setLeft(zoomAdjustedPixelValue(offset.left().value(), style.get()));
                region->m_isRectangle = (styleRegion.type == StyleDashboardRegion::Rectangle);
                region->m_isCircle = (styleRegion.type == StyleDashboardRegion::Circle);

                if (previousRegion)
                    previousRegion->m_next = region;
                else
                    firstRegion = region;
                previousRegion = region.get();
            }
            return cssValuePool().createValue(firstRegion.release());
        }
#endif
        case CSSPropertyWebkitAnimationDelay:
            return getDelayValue(style->animations());
        case CSSPropertyWebkitAnimationDirection: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->animations();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i) {
                    if (t->animation(i)->direction())
                        list->append(cssValuePool().createIdentifierValue(CSSValueAlternate));
                    else
                        list->append(cssValuePool().createIdentifierValue(CSSValueNormal));
                }
            } else
                list->append(cssValuePool().createIdentifierValue(CSSValueNormal));
            return list.release();
        }
        case CSSPropertyWebkitAnimationDuration:
            return getDurationValue(style->animations());
        case CSSPropertyWebkitAnimationFillMode: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->animations();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i) {
                    switch (t->animation(i)->fillMode()) {
                    case AnimationFillModeNone:
                        list->append(cssValuePool().createIdentifierValue(CSSValueNone));
                        break;
                    case AnimationFillModeForwards:
                        list->append(cssValuePool().createIdentifierValue(CSSValueForwards));
                        break;
                    case AnimationFillModeBackwards:
                        list->append(cssValuePool().createIdentifierValue(CSSValueBackwards));
                        break;
                    case AnimationFillModeBoth:
                        list->append(cssValuePool().createIdentifierValue(CSSValueBoth));
                        break;
                    }
                }
            } else
                list->append(cssValuePool().createIdentifierValue(CSSValueNone));
            return list.release();
        }
        case CSSPropertyWebkitAnimationIterationCount: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->animations();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i) {
                    double iterationCount = t->animation(i)->iterationCount();
                    if (iterationCount == Animation::IterationCountInfinite)
                        list->append(cssValuePool().createIdentifierValue(CSSValueInfinite));
                    else
                        list->append(cssValuePool().createValue(iterationCount, CSSPrimitiveValue::CSS_NUMBER));
                }
            } else
                list->append(cssValuePool().createValue(Animation::initialAnimationIterationCount(), CSSPrimitiveValue::CSS_NUMBER));
            return list.release();
        }
        case CSSPropertyWebkitAnimationName: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->animations();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i)
                    list->append(cssValuePool().createValue(t->animation(i)->name(), CSSPrimitiveValue::CSS_STRING));
            } else
                list->append(cssValuePool().createIdentifierValue(CSSValueNone));
            return list.release();
        }
        case CSSPropertyWebkitAnimationPlayState: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->animations();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i) {
                    int prop = t->animation(i)->playState();
                    if (prop == AnimPlayStatePlaying)
                        list->append(cssValuePool().createIdentifierValue(CSSValueRunning));
                    else
                        list->append(cssValuePool().createIdentifierValue(CSSValuePaused));
                }
            } else
                list->append(cssValuePool().createIdentifierValue(CSSValueRunning));
            return list.release();
        }
        case CSSPropertyWebkitAnimationTimingFunction:
            return getTimingFunctionValue(style->animations());
        case CSSPropertyWebkitAppearance:
            return cssValuePool().createValue(style->appearance());
        case CSSPropertyWebkitAspectRatio:
            if (!style->hasAspectRatio())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return CSSAspectRatioValue::create(style->aspectRatioNumerator(), style->aspectRatioDenominator());
        case CSSPropertyWebkitBackfaceVisibility:
            return cssValuePool().createIdentifierValue((style->backfaceVisibility() == BackfaceVisibilityHidden) ? CSSValueHidden : CSSValueVisible);
        case CSSPropertyWebkitBorderImage:
            return valueForNinePieceImage(style->borderImage());
        case CSSPropertyBorderImageOutset:
            return valueForNinePieceImageQuad(style->borderImage().outset());
        case CSSPropertyBorderImageRepeat:
            return valueForNinePieceImageRepeat(style->borderImage());
        case CSSPropertyBorderImageSlice:
            return valueForNinePieceImageSlice(style->borderImage());
        case CSSPropertyBorderImageWidth:
            return valueForNinePieceImageQuad(style->borderImage().borderSlices());
        case CSSPropertyWebkitMaskBoxImage:
            return valueForNinePieceImage(style->maskBoxImage());
        case CSSPropertyWebkitMaskBoxImageOutset:
            return valueForNinePieceImageQuad(style->maskBoxImage().outset());
        case CSSPropertyWebkitMaskBoxImageRepeat:
            return valueForNinePieceImageRepeat(style->maskBoxImage());
        case CSSPropertyWebkitMaskBoxImageSlice:
            return valueForNinePieceImageSlice(style->maskBoxImage());
        case CSSPropertyWebkitMaskBoxImageWidth:
            return valueForNinePieceImageQuad(style->maskBoxImage().borderSlices());
        case CSSPropertyWebkitMaskBoxImageSource:
            if (style->maskBoxImageSource())
                return style->maskBoxImageSource()->cssValue();
            return cssValuePool().createIdentifierValue(CSSValueNone);
        case CSSPropertyWebkitFontSizeDelta:
            break;
        case CSSPropertyWebkitMarginBottomCollapse:
        case CSSPropertyWebkitMarginAfterCollapse:
            return cssValuePool().createValue(style->marginAfterCollapse());
        case CSSPropertyWebkitMarginTopCollapse:
        case CSSPropertyWebkitMarginBeforeCollapse:
            return cssValuePool().createValue(style->marginBeforeCollapse());
#if ENABLE(OVERFLOW_SCROLLING)
        case CSSPropertyWebkitOverflowScrolling:
            if (!style->useTouchOverflowScrolling())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createIdentifierValue(CSSValueTouch);
#endif
        case CSSPropertyWebkitPerspective:
            if (!style->hasPerspective())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return zoomAdjustedPixelValue(style->perspective(), style.get());
        case CSSPropertyWebkitPerspectiveOrigin: {
            RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
            if (renderer) {
                LayoutRect box = sizingBox(renderer);
                RenderView* renderView = m_node->document()->renderView();
                list->append(zoomAdjustedPixelValue(minimumValueForLength(style->perspectiveOriginX(), box.width(), renderView), style.get()));
                list->append(zoomAdjustedPixelValue(minimumValueForLength(style->perspectiveOriginY(), box.height(), renderView), style.get()));
            }
            else {
                list->append(zoomAdjustedPixelValueForLength(style->perspectiveOriginX(), style.get()));
                list->append(zoomAdjustedPixelValueForLength(style->perspectiveOriginY(), style.get()));

            }
            return list.release();
        }
        case CSSPropertyWebkitRtlOrdering:
            return cssValuePool().createIdentifierValue(style->rtlOrdering() ? CSSValueVisual : CSSValueLogical);
#if ENABLE(TOUCH_EVENTS)
        case CSSPropertyWebkitTapHighlightColor:
            return currentColorOrValidColor(style.get(), style->tapHighlightColor());
#endif
        case CSSPropertyWebkitUserDrag:
            return cssValuePool().createValue(style->userDrag());
        case CSSPropertyWebkitUserSelect:
            return cssValuePool().createValue(style->userSelect());
        case CSSPropertyBorderBottomLeftRadius:
            return getBorderRadiusCornerValue(style->borderBottomLeftRadius(), style.get(), m_node->document()->renderView());
        case CSSPropertyBorderBottomRightRadius:
            return getBorderRadiusCornerValue(style->borderBottomRightRadius(), style.get(), m_node->document()->renderView());
        case CSSPropertyBorderTopLeftRadius:
            return getBorderRadiusCornerValue(style->borderTopLeftRadius(), style.get(), m_node->document()->renderView());
        case CSSPropertyBorderTopRightRadius:
            return getBorderRadiusCornerValue(style->borderTopRightRadius(), style.get(), m_node->document()->renderView());
        case CSSPropertyClip: {
            if (!style->hasClip())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            RefPtr<Rect> rect = Rect::create();
            rect->setTop(zoomAdjustedPixelValue(style->clip().top().value(), style.get()));
            rect->setRight(zoomAdjustedPixelValue(style->clip().right().value(), style.get()));
            rect->setBottom(zoomAdjustedPixelValue(style->clip().bottom().value(), style.get()));
            rect->setLeft(zoomAdjustedPixelValue(style->clip().left().value(), style.get()));
            return cssValuePool().createValue(rect.release());
        }
        case CSSPropertySpeak:
            return cssValuePool().createValue(style->speak());
        case CSSPropertyWebkitTransform:
            return computedTransform(renderer, style.get());
        case CSSPropertyWebkitTransformOrigin: {
            RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
            if (renderer) {
                LayoutRect box = sizingBox(renderer);
                RenderView* renderView = m_node->document()->renderView();
                list->append(zoomAdjustedPixelValue(minimumValueForLength(style->transformOriginX(), box.width(), renderView), style.get()));
                list->append(zoomAdjustedPixelValue(minimumValueForLength(style->transformOriginY(), box.height(), renderView), style.get()));
                if (style->transformOriginZ() != 0)
                    list->append(zoomAdjustedPixelValue(style->transformOriginZ(), style.get()));
            } else {
                list->append(zoomAdjustedPixelValueForLength(style->transformOriginX(), style.get()));
                list->append(zoomAdjustedPixelValueForLength(style->transformOriginY(), style.get()));
                if (style->transformOriginZ() != 0)
                    list->append(zoomAdjustedPixelValue(style->transformOriginZ(), style.get()));
            }
            return list.release();
        }
        case CSSPropertyWebkitTransformStyle:
            return cssValuePool().createIdentifierValue((style->transformStyle3D() == TransformStyle3DPreserve3D) ? CSSValuePreserve3d : CSSValueFlat);
        case CSSPropertyWebkitTransitionDelay:
            return getDelayValue(style->transitions());
        case CSSPropertyWebkitTransitionDuration:
            return getDurationValue(style->transitions());
        case CSSPropertyWebkitTransitionProperty: {
            RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
            const AnimationList* t = style->transitions();
            if (t) {
                for (size_t i = 0; i < t->size(); ++i) {
                    RefPtr<CSSValue> propertyValue;
                    const Animation* animation = t->animation(i);
                    if (animation->animationMode() == Animation::AnimateNone)
                        propertyValue = cssValuePool().createIdentifierValue(CSSValueNone);
                    else if (animation->animationMode() == Animation::AnimateAll)
                        propertyValue = cssValuePool().createIdentifierValue(CSSValueAll);
                    else
                        propertyValue = cssValuePool().createValue(getPropertyName(animation->property()), CSSPrimitiveValue::CSS_STRING);
                    list->append(propertyValue);
                }
            } else
                list->append(cssValuePool().createIdentifierValue(CSSValueAll));
            return list.release();
        }
        case CSSPropertyWebkitTransitionTimingFunction:
            return getTimingFunctionValue(style->transitions());
        case CSSPropertyPointerEvents:
            return cssValuePool().createValue(style->pointerEvents());
        case CSSPropertyWebkitColorCorrection:
            return cssValuePool().createValue(style->colorSpace());
        case CSSPropertyWebkitLineGrid:
            if (style->lineGrid().isNull())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return cssValuePool().createValue(style->lineGrid(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyWebkitLineSnap:
            return CSSPrimitiveValue::create(style->lineSnap());
        case CSSPropertyWebkitLineAlign:
            return CSSPrimitiveValue::create(style->lineAlign());
        case CSSPropertyWebkitWritingMode:
            return cssValuePool().createValue(style->writingMode());
        case CSSPropertyWebkitTextCombine:
            return cssValuePool().createValue(style->textCombine());
        case CSSPropertyWebkitTextOrientation:
            return CSSPrimitiveValue::create(style->fontDescription().textOrientation());
        case CSSPropertyWebkitLineBoxContain:
            return createLineBoxContainValue(style->lineBoxContain());
        case CSSPropertyContent:
            return contentToCSSValue(style.get());
        case CSSPropertyCounterIncrement:
            return counterToCSSValue(style.get(), propertyID);
        case CSSPropertyCounterReset:
            return counterToCSSValue(style.get(), propertyID);
#if ENABLE(CSS_REGIONS)
        case CSSPropertyWebkitFlowInto:
            if (style->flowThread().isNull())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return cssValuePool().createValue(style->flowThread(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyWebkitFlowFrom:
            if (style->regionThread().isNull())
                return cssValuePool().createIdentifierValue(CSSValueNone);
            return cssValuePool().createValue(style->regionThread(), CSSPrimitiveValue::CSS_STRING);
        case CSSPropertyWebkitRegionOverflow:
            return cssValuePool().createValue(style->regionOverflow());
#endif
#if ENABLE(CSS_EXCLUSIONS)
        case CSSPropertyWebkitWrapFlow:
            return cssValuePool().createValue(style->wrapFlow());
        case CSSPropertyWebkitWrapMargin:
            return cssValuePool().createValue(style->wrapMargin());
        case CSSPropertyWebkitWrapPadding:
            return cssValuePool().createValue(style->wrapPadding());
        case CSSPropertyWebkitShapeInside:
            if (!style->wrapShapeInside())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->wrapShapeInside());
        case CSSPropertyWebkitShapeOutside:
            if (!style->wrapShapeOutside())
                return cssValuePool().createIdentifierValue(CSSValueAuto);
            return cssValuePool().createValue(style->wrapShapeOutside());
        case CSSPropertyWebkitWrapThrough:
            return cssValuePool().createValue(style->wrapThrough());
#endif
#if ENABLE(CSS_FILTERS)
        case CSSPropertyWebkitFilter:
            return valueForFilter(style.get());
#endif
        case CSSPropertyBackground:
            return getBackgroundShorthandValue();
        case CSSPropertyBorder: {
            RefPtr<CSSValue> value = getPropertyCSSValue(CSSPropertyBorderTop, DoNotUpdateLayout);
            const CSSPropertyID properties[3] = { CSSPropertyBorderRight, CSSPropertyBorderBottom,
                                        CSSPropertyBorderLeft };
            for (size_t i = 0; i < WTF_ARRAY_LENGTH(properties); ++i) {
                if (value->cssText() !=  getPropertyCSSValue(properties[i], DoNotUpdateLayout)->cssText())
                    return 0;
            }
            return value.release();
        }
        case CSSPropertyBorderBottom:
            return getCSSPropertyValuesForShorthandProperties(borderBottomShorthand());
        case CSSPropertyBorderColor:
            return getCSSPropertyValuesForSidesShorthand(borderColorShorthand());
        case CSSPropertyBorderLeft:
            return getCSSPropertyValuesForShorthandProperties(borderLeftShorthand());
        case CSSPropertyBorderImage:
            return valueForNinePieceImage(style->borderImage());
        case CSSPropertyBorderRadius:
            return getBorderRadiusShorthandValue(style.get(), m_node->document()->renderView());
        case CSSPropertyBorderRight:
            return getCSSPropertyValuesForShorthandProperties(borderRightShorthand());
        case CSSPropertyBorderStyle:
            return getCSSPropertyValuesForSidesShorthand(borderStyleShorthand());
        case CSSPropertyBorderTop:
            return getCSSPropertyValuesForShorthandProperties(borderTopShorthand());
        case CSSPropertyBorderWidth:
            return getCSSPropertyValuesForSidesShorthand(borderWidthShorthand());
        case CSSPropertyListStyle:
            return getCSSPropertyValuesForShorthandProperties(listStyleShorthand());
        case CSSPropertyMargin:
            return getCSSPropertyValuesForSidesShorthand(marginShorthand());
        case CSSPropertyOutline:
            return getCSSPropertyValuesForShorthandProperties(outlineShorthand());
        case CSSPropertyPadding:
            return getCSSPropertyValuesForSidesShorthand(paddingShorthand());
        /* Individual properties not part of the spec */
        case CSSPropertyBackgroundRepeatX:
        case CSSPropertyBackgroundRepeatY:
            break;

        /* Unimplemented CSS 3 properties (including CSS3 shorthand properties) */
        case CSSPropertyWebkitTextEmphasis:
        case CSSPropertyTextLineThrough:
        case CSSPropertyTextLineThroughColor:
        case CSSPropertyTextLineThroughMode:
        case CSSPropertyTextLineThroughStyle:
        case CSSPropertyTextLineThroughWidth:
        case CSSPropertyTextOverline:
        case CSSPropertyTextOverlineColor:
        case CSSPropertyTextOverlineMode:
        case CSSPropertyTextOverlineStyle:
        case CSSPropertyTextOverlineWidth:
        case CSSPropertyTextUnderline:
        case CSSPropertyTextUnderlineColor:
        case CSSPropertyTextUnderlineMode:
        case CSSPropertyTextUnderlineStyle:
        case CSSPropertyTextUnderlineWidth:
            break;

        /* Directional properties are resolved by resolveDirectionAwareProperty() before the switch. */
        case CSSPropertyWebkitBorderEnd:
        case CSSPropertyWebkitBorderEndColor:
        case CSSPropertyWebkitBorderEndStyle:
        case CSSPropertyWebkitBorderEndWidth:
        case CSSPropertyWebkitBorderStart:
        case CSSPropertyWebkitBorderStartColor:
        case CSSPropertyWebkitBorderStartStyle:
        case CSSPropertyWebkitBorderStartWidth:
        case CSSPropertyWebkitBorderAfter:
        case CSSPropertyWebkitBorderAfterColor:
        case CSSPropertyWebkitBorderAfterStyle:
        case CSSPropertyWebkitBorderAfterWidth:
        case CSSPropertyWebkitBorderBefore:
        case CSSPropertyWebkitBorderBeforeColor:
        case CSSPropertyWebkitBorderBeforeStyle:
        case CSSPropertyWebkitBorderBeforeWidth:
        case CSSPropertyWebkitMarginEnd:
        case CSSPropertyWebkitMarginStart:
        case CSSPropertyWebkitMarginAfter:
        case CSSPropertyWebkitMarginBefore:
        case CSSPropertyWebkitPaddingEnd:
        case CSSPropertyWebkitPaddingStart:
        case CSSPropertyWebkitPaddingAfter:
        case CSSPropertyWebkitPaddingBefore:
        case CSSPropertyWebkitLogicalWidth:
        case CSSPropertyWebkitLogicalHeight:
        case CSSPropertyWebkitMinLogicalWidth:
        case CSSPropertyWebkitMinLogicalHeight:
        case CSSPropertyWebkitMaxLogicalWidth:
        case CSSPropertyWebkitMaxLogicalHeight:
            ASSERT_NOT_REACHED();
            break;

        /* Unimplemented @font-face properties */
        case CSSPropertyFontStretch:
        case CSSPropertySrc:
        case CSSPropertyUnicodeRange:
            break;

        /* Other unimplemented properties */
        case CSSPropertyPage: // for @page
        case CSSPropertyQuotes: // FIXME: needs implementation
        case CSSPropertySize: // for @page
            break;

        /* Unimplemented -webkit- properties */
        case CSSPropertyWebkitAnimation:
        case CSSPropertyWebkitBorderRadius:
        case CSSPropertyWebkitColumns:
        case CSSPropertyWebkitColumnRule:
        case CSSPropertyWebkitMarginCollapse:
        case CSSPropertyWebkitMarquee:
        case CSSPropertyWebkitMarqueeSpeed:
        case CSSPropertyWebkitMask:
        case CSSPropertyWebkitMaskRepeatX:
        case CSSPropertyWebkitMaskRepeatY:
        case CSSPropertyWebkitPerspectiveOriginX:
        case CSSPropertyWebkitPerspectiveOriginY:
        case CSSPropertyWebkitTextStroke:
        case CSSPropertyWebkitTransformOriginX:
        case CSSPropertyWebkitTransformOriginY:
        case CSSPropertyWebkitTransformOriginZ:
        case CSSPropertyWebkitTransition:
#if ENABLE(CSS_EXCLUSIONS)
        case CSSPropertyWebkitWrap:
#endif
            break;

#if ENABLE(SVG)
        case CSSPropertyClipPath:
        case CSSPropertyClipRule:
        case CSSPropertyMask:
        case CSSPropertyEnableBackground:
        case CSSPropertyFilter:
        case CSSPropertyFloodColor:
        case CSSPropertyFloodOpacity:
        case CSSPropertyLightingColor:
        case CSSPropertyStopColor:
        case CSSPropertyStopOpacity:
        case CSSPropertyColorInterpolation:
        case CSSPropertyColorInterpolationFilters:
        case CSSPropertyColorProfile:
        case CSSPropertyColorRendering:
        case CSSPropertyFill:
        case CSSPropertyFillOpacity:
        case CSSPropertyFillRule:
        case CSSPropertyMarker:
        case CSSPropertyMarkerEnd:
        case CSSPropertyMarkerMid:
        case CSSPropertyMarkerStart:
        case CSSPropertyShapeRendering:
        case CSSPropertyStroke:
        case CSSPropertyStrokeDasharray:
        case CSSPropertyStrokeDashoffset:
        case CSSPropertyStrokeLinecap:
        case CSSPropertyStrokeLinejoin:
        case CSSPropertyStrokeMiterlimit:
        case CSSPropertyStrokeOpacity:
        case CSSPropertyStrokeWidth:
        case CSSPropertyAlignmentBaseline:
        case CSSPropertyBaselineShift:
        case CSSPropertyDominantBaseline:
        case CSSPropertyGlyphOrientationHorizontal:
        case CSSPropertyGlyphOrientationVertical:
        case CSSPropertyKerning:
        case CSSPropertyTextAnchor:
        case CSSPropertyVectorEffect:
        case CSSPropertyWritingMode:
        case CSSPropertyWebkitSvgShadow:
            return getSVGPropertyCSSValue(propertyID, DoNotUpdateLayout);
#endif
    }

    logUnimplementedPropertyID(propertyID);
    return 0;
}
