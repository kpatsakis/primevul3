node_get_declared_uptime(const node_t *node)
{
  if (node->ri)
    return node->ri->uptime;
  else
    return -1;
}
