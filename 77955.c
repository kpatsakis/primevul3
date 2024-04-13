int CountRenderProcessHosts() {
  int result = 0;
  for (content::RenderProcessHost::iterator i(
          content::RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance())
    ++result;
  return result;
}
