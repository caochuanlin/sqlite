	//Sqlite数据库工具类
	//Sqlite对并发的支持不好，所以不要在多线程的情况下使用
	
	class SqliteTool
	{
	private:
		SqliteTool();
		~SqliteTool();
	
	public:
		/// \brief 打开数据库，若数据库不存在则直接创建
		static void OpenDataBase(const QString& strConnectionName);
		/// \brief 关闭数据库
		static void CloseDataBase();
		/// \brief 刷新数据库
		static void FlushDataBase();
		/// \brief 创建表
		static bool CreateTable( const QString& strTableName, const QString& strTableField );
		/// \brief 创建索引
		static bool CreateIndex( const QString& strIndexName, const QString& strTableName, const QString& strIndexField,bool bUnique = false );
		/// \brief 开启事务
		static void BeginTransaction();
		/// \brief 是否存在有效的数据库链接
		static bool HasValidConnection();
		/// \brief 获取数据库链接名称
		static QString GetConnectionName();
	
	private:
		static		QString				m_strConnectionName;					//< 数据库链接名称
	};
