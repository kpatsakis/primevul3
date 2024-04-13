    virtual void didFailProvisionalLoad(WebFrame* frame, const WebURLError& error)
    {
        frame->loadHTMLString("This should appear", toKURL("data:text/html,chromewebdata"), error.unreachableURL, true);
        runPendingTasks();
    }
