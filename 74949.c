bool RenderView::runFileChooser(
    const WebKit::WebFileChooserParams& params,
    WebFileChooserCompletion* chooser_completion) {
  ViewHostMsg_RunFileChooser_Params ipc_params;
  ipc_params.mode = params.multiSelect ?
      ViewHostMsg_RunFileChooser_Params::OpenMultiple :
      ViewHostMsg_RunFileChooser_Params::Open;
  ipc_params.title = params.title;
  ipc_params.default_file_name =
      webkit_glue::WebStringToFilePath(params.initialValue);

  return ScheduleFileChooser(ipc_params, chooser_completion);
}
