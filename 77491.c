char* ewk_frame_script_execute(Evas_Object* ewkFrame, const char* script)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(script, 0);

#if USE(JSC)
    WTF::String resultString;
    JSC::JSValue result = smartData->frame->script()->executeScript(WTF::String::fromUTF8(script), true).jsValue();

    if (!smartData->frame) // In case the script removed our frame from the page.
        return 0;

    if (!result || (!result.isBoolean() && !result.isString() && !result.isNumber()))
        return 0;

    JSC::JSLock lock(JSC::SilenceAssertionsOnly);
    JSC::ExecState* exec = smartData->frame->script()->globalObject(WebCore::mainThreadNormalWorld())->globalExec();
    resultString = WebCore::ustringToString(result.toString(exec)->value(exec));
    return strdup(resultString.utf8().data());
#else
    notImplemented();
    return 0;
#endif
}
