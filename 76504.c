Authenticator* LoginUtilsImpl::CreateAuthenticator(
    LoginStatusConsumer* consumer) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kParallelAuth))
    return new ParallelAuthenticator(consumer);
  else
    return new GoogleAuthenticator(consumer);
}
