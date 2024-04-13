void ScriptController::clearForClose()
{
    double start = currentTime();
    clearForClose(false);
    HistogramSupport::histogramCustomCounts("WebCore.ScriptController.clearForClose", (currentTime() - start) * 1000, 0, 10000, 50);
}
