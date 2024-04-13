void TabsUpdateFunction::PopulateResult() {
  if (!has_callback())
    return;

  SetResult(ExtensionTabUtil::CreateTabValue(web_contents_, GetExtension()));
}
