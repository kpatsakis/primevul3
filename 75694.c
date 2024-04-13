static PassRefPtr<CSSValue> getTimingFunctionValue(const AnimationList* animList)
{
    RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
    if (animList) {
        for (size_t i = 0; i < animList->size(); ++i) {
            const TimingFunction* tf = animList->animation(i)->timingFunction().get();
            if (tf->isCubicBezierTimingFunction()) {
                const CubicBezierTimingFunction* ctf = static_cast<const CubicBezierTimingFunction*>(tf);
                list->append(CSSCubicBezierTimingFunctionValue::create(ctf->x1(), ctf->y1(), ctf->x2(), ctf->y2()));
            } else if (tf->isStepsTimingFunction()) {
                const StepsTimingFunction* stf = static_cast<const StepsTimingFunction*>(tf);
                list->append(CSSStepsTimingFunctionValue::create(stf->numberOfSteps(), stf->stepAtStart()));
            } else {
                list->append(CSSLinearTimingFunctionValue::create());
            }
        }
    } else {
        RefPtr<TimingFunction> tf = Animation::initialAnimationTimingFunction();
        if (tf->isCubicBezierTimingFunction()) {
            const CubicBezierTimingFunction* ctf = static_cast<const CubicBezierTimingFunction*>(tf.get());
            list->append(CSSCubicBezierTimingFunctionValue::create(ctf->x1(), ctf->y1(), ctf->x2(), ctf->y2()));
        } else if (tf->isStepsTimingFunction()) {
            const StepsTimingFunction* stf = static_cast<const StepsTimingFunction*>(tf.get());
            list->append(CSSStepsTimingFunctionValue::create(stf->numberOfSteps(), stf->stepAtStart()));
        } else {
            list->append(CSSLinearTimingFunctionValue::create());
        }
    }
    return list.release();
}
