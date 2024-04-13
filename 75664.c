static PassRefPtr<CSSValue> computedTransform(RenderObject* renderer, const RenderStyle* style)
{
    if (!renderer || style->transform().operations().isEmpty())
        return cssValuePool().createIdentifierValue(CSSValueNone);

    LayoutRect box = sizingBox(renderer);

    TransformationMatrix transform;
    style->applyTransform(transform, box.size(), RenderStyle::ExcludeTransformOrigin);

    RefPtr<WebKitCSSTransformValue> transformVal;

    if (transform.isAffine()) {
        transformVal = WebKitCSSTransformValue::create(WebKitCSSTransformValue::MatrixTransformOperation);

        transformVal->append(cssValuePool().createValue(transform.a(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.b(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.c(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.d(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(zoomAdjustedNumberValue(transform.e(), style));
        transformVal->append(zoomAdjustedNumberValue(transform.f(), style));
    } else {
        transformVal = WebKitCSSTransformValue::create(WebKitCSSTransformValue::Matrix3DTransformOperation);

        transformVal->append(cssValuePool().createValue(transform.m11(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m12(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m13(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m14(), CSSPrimitiveValue::CSS_NUMBER));

        transformVal->append(cssValuePool().createValue(transform.m21(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m22(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m23(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m24(), CSSPrimitiveValue::CSS_NUMBER));

        transformVal->append(cssValuePool().createValue(transform.m31(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m32(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m33(), CSSPrimitiveValue::CSS_NUMBER));
        transformVal->append(cssValuePool().createValue(transform.m34(), CSSPrimitiveValue::CSS_NUMBER));

        transformVal->append(zoomAdjustedNumberValue(transform.m41(), style));
        transformVal->append(zoomAdjustedNumberValue(transform.m42(), style));
        transformVal->append(zoomAdjustedNumberValue(transform.m43(), style));
        transformVal->append(cssValuePool().createValue(transform.m44(), CSSPrimitiveValue::CSS_NUMBER));
    }

    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    list->append(transformVal);

    return list.release();
}
