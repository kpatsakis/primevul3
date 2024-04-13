		void CWebServer::CleanSessions() {
			m_sql.safe_query(
				"DELETE FROM UserSessions WHERE ExpirationDate < datetime('now', 'localtime')");
		}
