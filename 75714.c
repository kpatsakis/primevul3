PassRefPtr<CSSValue> CSSComputedStyleDeclaration::valueForFilter(RenderStyle* style) const
{
    if (style->filter().operations().isEmpty())
        return cssValuePool().createIdentifierValue(CSSValueNone);

    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();

    RefPtr<WebKitCSSFilterValue> filterValue;

    Vector<RefPtr<FilterOperation> >::const_iterator end = style->filter().operations().end();
    for (Vector<RefPtr<FilterOperation> >::const_iterator it = style->filter().operations().begin(); it != end; ++it) {
        FilterOperation* filterOperation = (*it).get();
        switch (filterOperation->getOperationType()) {
        case FilterOperation::REFERENCE: {
            ReferenceFilterOperation* referenceOperation = static_cast<ReferenceFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::ReferenceFilterOperation);
            filterValue->append(cssValuePool().createValue(referenceOperation->reference(), CSSPrimitiveValue::CSS_STRING));
            break;
        }
        case FilterOperation::GRAYSCALE: {
            BasicColorMatrixFilterOperation* colorMatrixOperation = static_cast<BasicColorMatrixFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::GrayscaleFilterOperation);
            filterValue->append(cssValuePool().createValue(colorMatrixOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::SEPIA: {
            BasicColorMatrixFilterOperation* colorMatrixOperation = static_cast<BasicColorMatrixFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::SepiaFilterOperation);
            filterValue->append(cssValuePool().createValue(colorMatrixOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::SATURATE: {
            BasicColorMatrixFilterOperation* colorMatrixOperation = static_cast<BasicColorMatrixFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::SaturateFilterOperation);
            filterValue->append(cssValuePool().createValue(colorMatrixOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::HUE_ROTATE: {
            BasicColorMatrixFilterOperation* colorMatrixOperation = static_cast<BasicColorMatrixFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::HueRotateFilterOperation);
            filterValue->append(cssValuePool().createValue(colorMatrixOperation->amount(), CSSPrimitiveValue::CSS_DEG));
            break;
        }
        case FilterOperation::INVERT: {
            BasicComponentTransferFilterOperation* componentTransferOperation = static_cast<BasicComponentTransferFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::InvertFilterOperation);
            filterValue->append(cssValuePool().createValue(componentTransferOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::OPACITY: {
            BasicComponentTransferFilterOperation* componentTransferOperation = static_cast<BasicComponentTransferFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::OpacityFilterOperation);
            filterValue->append(cssValuePool().createValue(componentTransferOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::BRIGHTNESS: {
            BasicComponentTransferFilterOperation* brightnessOperation = static_cast<BasicComponentTransferFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::BrightnessFilterOperation);
            filterValue->append(cssValuePool().createValue(brightnessOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::CONTRAST: {
            BasicComponentTransferFilterOperation* contrastOperation = static_cast<BasicComponentTransferFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::ContrastFilterOperation);
            filterValue->append(cssValuePool().createValue(contrastOperation->amount(), CSSPrimitiveValue::CSS_NUMBER));
            break;
        }
        case FilterOperation::BLUR: {
            BlurFilterOperation* blurOperation = static_cast<BlurFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::BlurFilterOperation);
            filterValue->append(zoomAdjustedPixelValue(blurOperation->stdDeviation().value(), style));
            break;
        }
        case FilterOperation::DROP_SHADOW: {
            DropShadowFilterOperation* dropShadowOperation = static_cast<DropShadowFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::DropShadowFilterOperation);
            ShadowData shadowData = ShadowData(dropShadowOperation->location(), dropShadowOperation->stdDeviation(), 0, Normal, false, dropShadowOperation->color());
            filterValue->append(valueForShadow(&shadowData, CSSPropertyTextShadow, style));
            break;
        }
#if ENABLE(CSS_SHADERS)
        case FilterOperation::CUSTOM: {
            CustomFilterOperation* customOperation = static_cast<CustomFilterOperation*>(filterOperation);
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::CustomFilterOperation);
            
            
            ASSERT(customOperation->program());
            StyleCustomFilterProgram* program = static_cast<StyleCustomFilterProgram*>(customOperation->program());
            
            RefPtr<CSSValueList> shadersList = CSSValueList::createSpaceSeparated();
            if (program->vertexShader())
                shadersList->append(program->vertexShader()->cssValue());
            else
                shadersList->append(cssValuePool().createIdentifierValue(CSSValueNone));
            if (program->fragmentShader())
                shadersList->append(program->fragmentShader()->cssValue());
            else
                shadersList->append(cssValuePool().createIdentifierValue(CSSValueNone));
            filterValue->append(shadersList.release());
            
            RefPtr<CSSValueList> meshParameters = CSSValueList::createSpaceSeparated();
            meshParameters->append(cssValuePool().createValue(customOperation->meshRows(), CSSPrimitiveValue::CSS_NUMBER));
            meshParameters->append(cssValuePool().createValue(customOperation->meshColumns(), CSSPrimitiveValue::CSS_NUMBER));
            meshParameters->append(cssValuePool().createValue(customOperation->meshBoxType()));
            
            if (customOperation->meshType() == CustomFilterOperation::DETACHED)
                meshParameters->append(cssValuePool().createIdentifierValue(CSSValueDetached));
            
            filterValue->append(meshParameters.release());
            
            const CustomFilterParameterList& parameters = customOperation->parameters();
            size_t parametersSize = parameters.size();
            if (!parametersSize)
                break;
            RefPtr<CSSValueList> parametersCSSValue = CSSValueList::createCommaSeparated();
            for (size_t i = 0; i < parametersSize; ++i) {
                const CustomFilterParameter* parameter = parameters.at(i).get();
                RefPtr<CSSValueList> parameterCSSNameAndValue = CSSValueList::createSpaceSeparated();
                parameterCSSNameAndValue->append(cssValuePool().createValue(parameter->name(), CSSPrimitiveValue::CSS_STRING));
                parameterCSSNameAndValue->append(valueForCustomFilterParameter(parameter));
                parametersCSSValue->append(parameterCSSNameAndValue.release());
            }
            
            filterValue->append(parametersCSSValue.release());
            break;
        }
#endif
        default:
            filterValue = WebKitCSSFilterValue::create(WebKitCSSFilterValue::UnknownFilterOperation);
            break;
        }
        list->append(filterValue);
    }

    return list.release();
}
