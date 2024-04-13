void ewk_view_dispatch_did_finish_loading(Evas_Object* ewkView)
{
    /* If we reach this point and rendering is still disabled, WebCore will not
     * trigger the didFirstVisuallyNonEmptyLayout signal anymore. So, we
     * forcefully re-enable the rendering.
     */
    ewk_view_did_first_visually_nonempty_layout(ewkView);
}
