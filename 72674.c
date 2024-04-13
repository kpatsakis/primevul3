void hide_shell(u32 cmd_type)
{
#if defined(WIN32) && !defined(_WIN32_WCE)
	typedef HWND (WINAPI *GetConsoleWindowT)(void);
	HMODULE hk32 = GetModuleHandle("kernel32.dll");
	GetConsoleWindowT GetConsoleWindow = (GetConsoleWindowT ) GetProcAddress(hk32,"GetConsoleWindow");
	if (cmd_type==0) {
		ShowWindow( GetConsoleWindow(), SW_SHOW);
		shell_visible = GF_TRUE;
	}
	else if (cmd_type==1) {
		ShowWindow( GetConsoleWindow(), SW_HIDE);
		shell_visible = GF_FALSE;
	}
	else if (cmd_type==2) PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);

#endif
}
