#ifndef _MYCONNECTION_POOL_H  
#define _MYCONNECTION_POOL_H  
#include<mysql_connection.h>  
#include<mysql_driver.h>  
#include<cppconn/exception.h>  
#include<cppconn/driver.h>  
#include<cppconn/connection.h>  
#include<cppconn/resultset.h>  
#include<cppconn/prepared_statement.h>  
#include<cppconn/statement.h>  
#include<Poco/Thread.h>  
#include<list>  

using namespace std;  
using namespace sql;  

namespace Pheux { namespace Core {
  class ConnPool{  
    private:  
      int curSize;//当前已建立的数据库连接数量  
      int maxSize;//连接池中定义的最大数据库连接数  
      string username;  
      string password;  
      string url;  
      list<Connection*> connList;//连接池的容器队列  
      Poco::Mutex _mutex;;//线程锁  
      static ConnPool *connPool;  
      Driver* driver;  

      Connection*CreateConnection();//创建一个连接  
      void InitConnection(int iInitialSize);//初始化数据库连接池  
      void DestoryConnection(Connection *conn);//销毁数据库连接对象  
      void DestoryConnPool();//销毁数据库连接池  
      ConnPool(string url,string user,string password,int maxSize);//构造方法  
      ConnPool(const string &cfg_filename);//构造方法

    public:  
      ~ConnPool();  
      Connection*GetConnection();//获得数据库连接  
      void ReleaseConnection(Connection *conn);//将数据库连接放回到连接池的容器中  
      static ConnPool *GetInstance();//获取数据库连接池对象  
  };  
}}
#endif
