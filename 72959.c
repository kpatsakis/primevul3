child_cleanup(int signo _U_)
{
  wait(NULL);
}
