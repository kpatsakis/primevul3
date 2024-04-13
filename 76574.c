void ChromotingInstance::SetDesktopSize(const SkISize& size,
                                        const SkIPoint& dpi) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetInteger("width", size.width());
  data->SetInteger("height", size.height());
  if (dpi.x())
    data->SetInteger("x_dpi", dpi.x());
  if (dpi.y())
    data->SetInteger("y_dpi", dpi.y());
  PostChromotingMessage("onDesktopSize", data.Pass());
}
