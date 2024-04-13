		void CWebServer::Cmd_UpdateDevices(WebEmSession & session, const request& req, Json::Value &root)
		{
			std::string script = request::findValue(&req, "script");
			if (script.empty())
			{
				return;
			}
			std::string content = req.content;

			std::vector<std::string> allParameters;

			std::vector<std::string> allParts;
			StringSplit(req.uri, "?", allParts);
			if (!allParts.empty())
			{
				StringSplit(allParts[1], "&", allParameters);
			}

			CLuaHandler luaScript;
			bool ret = luaScript.executeLuaScript(script, content, allParameters);
			if (ret)
			{
				root["status"] = "OK";
				root["title"] = "Update Device";
			}
		}
