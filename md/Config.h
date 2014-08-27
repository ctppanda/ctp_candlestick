#ifndef PHEUX_NEWCONFIG_H
#define PHEUX_NEWCONFIG_H

#include "Util.h"
#include "Account.h"

namespace Pheux { namespace Core {
 //class Account;
 class Config
 {
 public:
     Config(const string& file_name);
     ~Config() {}
	 static Config* cfg;

     void   parse_xml();
	 string Getmysql_port();
	 string Getmysql_tcp();
	 string Getmysql_password();
	 string Getusername();
	 int    Getmaxsize();
public:
     static Config* Instance();
     Account account;
     vector<string> InstrumentID;
     string cfgxml_name;
	 string mysql_port;
	 string mysql_tcp;
	 string mysql_password;
	 string mysql_username;
	 int    maxsize;
 };

}}
#endif
