#include "Config.h"
#include "Util.h"
using namespace tinyxml2;

namespace Pheux { namespace Core {

	Config * Config::cfg = NULL;

Config::Config(const string& file_name){
	cfgxml_name = file_name;
	parse_xml();
}
string Config::Getmysql_port()
{
	return mysql_port;
}
string Config::Getmysql_tcp()
{
	return mysql_tcp;
}
string Config::Getmysql_password()
{
	return mysql_password;
}
string Config::Getusername()
{
	return mysql_username;
}
int Config::Getmaxsize()
{
	return maxsize;
}

void Config::parse_xml()
{
  XMLDocument doc;
  doc.LoadFile( cfgxml_name.c_str() );
  XMLElement *cfg=doc.RootElement();
  XMLElement *accounts=cfg->FirstChildElement("Accounts");
  XMLElement *accnt=accounts->FirstChildElement("Account");
  while (accnt)
  {
    XMLElement *accntChild=accnt->FirstChildElement();
    const char* content;
    const XMLAttribute *attributeOfaccnt = accnt->FirstAttribute();
    //attributeOfaccnt->Value() this must be unique.
    cout<< attributeOfaccnt->Name() << ":" << attributeOfaccnt->Value() << endl;
	string cmp_string;
    while(accntChild)
    {
      //const XMLAttribute *attributeOfaccntchild = accntChild->FirstAttribute();
      //cout << attributeOfaccntchild->Name() << endl;
      content=accntChild->GetText();
      cout<<accntChild->Name()<<endl;
	  cmp_string = accntChild->Name();
      //cout<<accntChild->Value()<<endl;
      
      accntChild=accntChild->NextSiblingElement();
	  if (cmp_string == "MarketAddress")
		  account.MarketAddress = content;
	  else if (cmp_string == "TraderAddress")
		  account.TraderAddress = content;
	  else if (cmp_string == "Broker")
		  account.BrokerID = content;
	  else if (cmp_string == "Investor")
		  account.AccountID = content;
	  else if (cmp_string == "Password")
		  account.password = content;
	  else if (cmp_string == "contract")
		  InstrumentID.push_back(content);
	  else if (cmp_string == "sqlhost")
		  mysql_tcp = content;
	  else if (cmp_string == "sqlusername")
		  mysql_username = content;
	  else if (cmp_string == "sqlpassword")
	      mysql_password = content;
	  else if (cmp_string == "sqlport")
	      mysql_port = content;
	  else if (cmp_string == "sqlmaxcon") {
		  cout << content << endl;
		  maxsize    = Converter::ToInt(content);
	  }
      cout<<content<<endl;
    }
    accnt=accnt->NextSiblingElement();
  }
  //XMLElement *instrmnts=cfg->FirstChildElement("instruments");
  //cout << instrmnts->GetText() << endl;
  // XMLElement *contract=instrmnts->FirstChildElement("contract");
  //cout << "fuck" << endl;
  //while (contract)
  //{
  //  XMLElement *contractChild=contract->FirstChildElement();
  // const char* content;
  //  const XMLAttribute *attributeOfcontract = contract->FirstAttribute();
  //   cout<< attributeOfcontract->Name() << ":" << attributeOfcontract->Value() << endl;
  // while(contractChild)
  // {
  //   //const XMLAttribute *attributeOfcontractchild = contractChild->FirstAttribute();
  //   //cout << attributeOfcontractchild->Name() << endl;
  //   cout <<"here2" << endl;
  //   content=contractChild->GetText();
  //   cout<<contractChild->Name()<<endl;
  //   cout<<contractChild->Value()<<endl;
  //  InstrumentID.push_back(content);
  //  contractChild=contractChild->NextSiblingElement();
  //   cout<<content<<endl;
  // }
  // contract=contract->NextSiblingElement();
  // }
}
Config* Config::Instance()
{
  if (cfg == NULL)
    cfg = new Config("config.xml");
  return cfg;
}
}}




