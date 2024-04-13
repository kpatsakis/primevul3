  PendingFileChooser(const ViewHostMsg_RunFileChooser_Params& p,
                     WebFileChooserCompletion* c)
      : params(p),
        completion(c) {
  }
