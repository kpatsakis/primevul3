		void CWebServer::AddUser(const unsigned long ID, const std::string &username, const std::string &password, const int userrights, const int activetabs)
		{
			std::vector<std::vector<std::string> > result = m_sql.safe_query("SELECT COUNT(*) FROM SharedDevices WHERE (SharedUserID == '%d')", ID);
			if (result.empty())
				return;

			_tWebUserPassword wtmp;
			wtmp.ID = ID;
			wtmp.Username = username;
			wtmp.Password = password;
			wtmp.userrights = (_eUserRights)userrights;
			wtmp.ActiveTabs = activetabs;
			wtmp.TotSensors = atoi(result[0][0].c_str());
			m_users.push_back(wtmp);

			m_pWebEm->AddUserPassword(ID, username, password, (_eUserRights)userrights, activetabs);
		}
