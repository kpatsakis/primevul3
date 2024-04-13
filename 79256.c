bool TabsGetCurrentFunction::RunImpl() {
  DCHECK(dispatcher());

  WebContents* contents = dispatcher()->delegate()->GetAssociatedWebContents();
  if (contents)
    SetResult(ExtensionTabUtil::CreateTabValue(contents, GetExtension()));

  return true;
}
