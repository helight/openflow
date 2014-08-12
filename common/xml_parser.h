#include<iostream>
#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace boost::property_tree;

static const string task_member[]={"name","description","nodes","command"};

struct StTask
{
	std::string name;
	std::string descroption;
	std::string nodes;
	std::string cmd;
};

class xmlParser
{
	private:
		ptree pt;
		ptree root;
		StTask task;
	public:
		vector<StTask> parseXML(string xmlFile);
		bool format_Ck(string xmlFile);
		int shellPrcss_num(string xmlFile);
};

vector<StTask> xmlParser::parseXML(string xmlFile)
{
	vector<StTask> rsltTasks;
	read_xml(xmlFile,pt);
	root=pt.get_child("job");

	for(ptree::iterator it=root.begin();it!=root.end();it++)
	{
		if(it->first=="shell_process")
		{
			ptree pt;
			string val;
			ptree shell_process=it->second;

			int i=0;
			for(;i<4;i++)
			{
				pt=shell_process.get_child(task_member[i]);
				val=pt.data();
				cout<<val<<endl;
			}

			switch(i)
			{
				case 0:
					task.name=val;
					break;
				case 1:
					task.description=val;
					break;
				case 2:
					task.nodes=val;
					break;
				case 3:
					task.cmd=val;
					break;
			}

			rsltTasks.push_back(task);
		}
	}
	return rsltTasks;
}

bool xmlParser::format_Ck(string xmlFile)
{
	ptree stdF;
	ptree stdF_root;
	
	read_xml(xmlFile,pt);
	root=pt.get_child("job");

	read_xml("standard.xml",stdF);
	stdF_root=stdF.get_child("job");

	ptree::iterator it=root.begin();
	ptree::iteratro std_it=stdF_root.begin();

	while(it!=root.end())
	{
		if(it->first!="shell_process")
		{
			if(it->first!=std_it->first)
				return false;
		}
		else
		{
			it++;
			continue;
		}
		it++;
		std_it++;
	}
	it(std_it!=stdF_root.end())
		return false;
	return true;
}

int xmlParser::shellPrcss_num(string xmlFile)
{
	read_xml(xmlFile,pt);
	root=pt.get_child("job");

	int dscrptnCount=0;
	
	ptree::iterator it=root.begin();

	while(it!=root.end())
	{
		if(it->first=="shell_process")
			dscrptnCount++;
		it++;
	}

	return dscrptnCount;
}
