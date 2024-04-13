int __cdecl DebugOnStart::Init() {
  if (FindArgument(GetCommandLine(), switches::kDebugOnStart)) {

    base::debug::SpawnDebuggerOnProcess(GetCurrentProcessId());

    base::debug::WaitForDebugger(60, false);
  } else if (FindArgument(GetCommandLine(), switches::kWaitForDebugger)) {
    base::debug::WaitForDebugger(60, true);
  }
  return 0;
}
