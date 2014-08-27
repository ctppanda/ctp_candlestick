#include<stdexcept>  
#include<exception>  
#include<stdio.h> 
#include "Config.h"
#include"ConnectionPool.h"  


using namespace std;  
using namespace sql;  
namespace Pheux { namespace Core {
  ConnPool*ConnPool::connPool=NULL;  
  //连接池的构造函数
  ConnPool::ConnPool(const string& cfg_filename)  
  {  
    Config::Instance()->parse_xml();
    this->maxSize=Config::Instance()->Getmaxsize();
    this->curSize=0;  
    this->username=Config::Instance()->Getusername();
    this->password=Config::Instance()->Getmysql_password();  
    this->url=Config::Instance()->Getmysql_tcp() + ":" + Config::Instance()->Getmysql_port();
    try{  
      this->driver=sql::mysql::get_driver_instance();  
    }  
    catch(sql::SQLException &e)  
    {  
      perror("驱动连接出错;\n");  
    }  
    catch(std::runtime_error &e)  
    {  
      perror("运行出错了\n");  
    }  
    this->InitConnection(maxSize/2);  
  }  
  ConnPool::ConnPool(string url, string userName,string password, int maxSize)  
  {  
    this->maxSize=maxSize;  
    this->curSize=0;  
    this->username=userName;  
    this->password=password;  
    this->url=url;  
    try{  
      this->driver=sql::mysql::get_driver_instance();  
    }  
    catch(sql::SQLException &e)  
    {  
      perror("驱动连接出错;\n");  
    }  
    catch(std::runtime_error &e)  
    {  
      perror("运行出错了\n");  
    }  
    this->InitConnection(maxSize/2);  
  }  
  //获取连接池对象，单例模式  
  ConnPool*ConnPool::GetInstance(){  
    if(connPool==NULL)  
    {  
      connPool=new ConnPool("config.xml");  
    }  
    return connPool;  
  }  
  //初始化连接池，创建最大连接数的一半连接数量  
  void ConnPool::InitConnection(int iInitialSize)  
  {  
    Connection*conn;  
    _mutex.lock();  
    for(int i=0;i<iInitialSize;i++)  
    {  
      conn=this->CreateConnection();  
      if(conn){  
        connList.push_back(conn);  
        ++(this->curSize);  
      }  
      else  
      {  
        perror("创建CONNECTION出错");  
      }  
    }  
    _mutex.unlock();  
  }  
  //创建连接,返回一个Connection  
  Connection*ConnPool::CreateConnection(){  
    Connection*conn;  
    try{
		cout << this->url << endl;
	    cout << this->username << endl;
		cout << this->password <<endl;
      conn=driver->connect(this->url.c_str(),this->username.c_str(),this->password.c_str());//建立连接 
	  cout << "one more is done！" <<endl;
      return conn;  
    }  
    catch(sql::SQLException &e)  
    {  
      cout << "error1" <<endl;  
      return NULL;  
    }  
    catch(std::runtime_error &e)  
    {  
      cout << "error2" <<endl;  
      return NULL;  
    }  
  }  
  //在连接池中获得一个连接  
  Connection*ConnPool::GetConnection(){  
    Connection*con;  
    _mutex.lock();  
    if(connList.size()>0)//连接池容器中还有连接  
    {  
      con=connList.front();//得到第一个连接  
      connList.pop_front();//移除第一个连接  
      if(con->isClosed())//如果连接已经被关闭，删除后重新建立一个  
      {  
        delete con;  
        con=this->CreateConnection();  
      }  
      //如果连接为空，则创建连接出错  
      if(con==NULL)  
      {  
        --curSize;  
      }  
      _mutex.unlock();  
      return con;  
    }  
    else{  
      if(curSize< maxSize){//还可以创建新的连接  
        con= this->CreateConnection();  
        if(con){  
          ++curSize;  
          _mutex.unlock();  
          return con;  
        }  
        else{  
          _mutex.unlock();  
          return NULL;  
        }  
      }  
      else{//建立的连接数已经达到maxSize  
        _mutex.unlock();  
        return NULL;  
      }  
    }  
  }  
  //回收数据库连接  
  void ConnPool::ReleaseConnection(sql::Connection * conn){  
    if(conn){  
      _mutex.lock();  
      connList.push_back(conn);  
      _mutex.unlock();  
    }  
  }  
  //连接池的析构函数  
  ConnPool::~ConnPool()  
  {  
    this->DestoryConnPool();  
  }  
  //销毁连接池,首先要先销毁连接池的中连接  
  void ConnPool::DestoryConnPool(){  
    list<Connection*>::iterator icon;  
    _mutex.lock();  
    for(icon=connList.begin();icon!=connList.end();++icon)  
    {  
      this->DestoryConnection(*icon);//销毁连接池中的连接  
    }  
    curSize=0;  
    connList.clear();//清空连接池中的连接  
    _mutex.unlock();  
  }  
  //销毁一个连接  
  void ConnPool::DestoryConnection(Connection* conn)  
  {  
    if(conn)  
    {  
      try{  
        conn->close();  
      }  
      catch(sql::SQLException&e)  
      {  
        perror(e.what());  
      }  
      catch(std::exception&e)  
      {  
        perror(e.what());  
      }  
      delete conn;  
    }  
  } 

}}
