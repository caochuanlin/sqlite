	QString SqliteTool::m_strConnectionName;

	SqliteTool::SqliteTool()
	{

	}

	SqliteTool::~SqliteTool()
	{

	}

	void SqliteTool::OpenDataBase( const QString& strConnectionName )
	{
		m_strConnectionName = strConnectionName;

		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",strConnectionName);
		db.setDatabaseName(strConnectionName);

		//打开数据库，若数据库文件不存在则创建
		if (!db.open())
		{
			qDebug()<<db.lastError().text();
		}
	}

	void SqliteTool::CloseDataBase()
	{
		if (QSqlDatabase::contains(m_strConnectionName))
		{
			{
				QSqlDatabase db = QSqlDatabase::database(m_strConnectionName);
				if (db.isValid())
				{
					if (db.isOpen())
					{
						db.close();
					}
				}
			}

			QSqlDatabase::removeDatabase(m_strConnectionName);
		}
	}

	void SqliteTool::BeginTransaction()
	{
		if (QSqlDatabase::contains(m_strConnectionName))
		{
			QSqlDatabase db = QSqlDatabase::database(m_strConnectionName);
			if (db.isValid())
			{
				if (db.isOpen())
				{
					db.transaction();
				}
			}
		}
	}

	void SqliteTool::FlushDataBase()
	{
		if (QSqlDatabase::contains(m_strConnectionName))
		{
			QSqlDatabase db = QSqlDatabase::database(m_strConnectionName);
			if (db.isValid())
			{
				if (db.isOpen())
				{
					db.transaction();

					if (!db.commit())
					{
						qDebug()<<db.lastError().text();
					}
				}
			}
		}
	}

	bool SqliteTool::CreateTable( const QString& strTableName, const QString& strTableField )
	{
		if (QSqlDatabase::contains(m_strConnectionName))
		{
			QSqlDatabase db = QSqlDatabase::database(m_strConnectionName);
			if (db.isValid())
			{
				if (db.isOpen())
				{
					QSqlQuery sql_query(db);

					QString strIsTableExists = QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(strTableName);

					if (!sql_query.prepare(strIsTableExists))
					{
						qDebug()<<sql_query.lastError().text();

						return  false;
					}

					if(!sql_query.exec())
					{
						qDebug()<<sql_query.lastError().text();

						return false;
					}

					if (!sql_query.next())
					{
						qDebug()<<sql_query.lastError().text();

						return false;
					}

					if(sql_query.value(0).toInt() == 0)  //not exist
					{
						QString strCreateTable = QString("create table %1 (%2)").arg(strTableName).arg(strTableField);

						if (!sql_query.prepare(strCreateTable))
						{
							qDebug()<<sql_query.lastError().text();

							return  false;
						}

						if(!sql_query.exec())
						{
							qDebug()<<sql_query.lastError().text();

							return  false;
						}
					}
				}
			}
		}

		return true;
	}

	bool SqliteTool::CreateIndex( const QString& strIndexName, const QString& strTableName, const QString& strIndexField,bool bUnique /*= false*/ )
	{
		if (QSqlDatabase::contains(m_strConnectionName))
		{
			QSqlDatabase db = QSqlDatabase::database(m_strConnectionName);
			if (db.isValid())
			{
				if (db.isOpen())
				{
					QString strCreateIndex;
					if (bUnique)
					{
						strCreateIndex = QString("create unique index %1 on %2 (%3)").arg(strIndexName).arg(strTableName).arg(strIndexField);
					}
					else
					{
						strCreateIndex = QString("create index %1 on %2 (%3)").arg(strIndexName).arg(strTableName).arg(strIndexField);
					}

					QSqlQuery sql_query(db);

					if (!sql_query.prepare(strCreateIndex))
					{
						qDebug()<<sql_query.lastError().text();

						return  false;
					}

					if(!sql_query.exec())
					{
						qDebug()<<sql_query.lastError().text();

						return  false;
					}
				}
			}
		}

		return true;
	}

	bool SqliteTool::HasValidConnection()
	{
		if (m_strConnectionName.isEmpty())
		{
			return false;
		}

		return QSqlDatabase::contains(m_strConnectionName);
	}

	QString SqliteTool::GetConnectionName()
	{
		return m_strConnectionName;
	}
