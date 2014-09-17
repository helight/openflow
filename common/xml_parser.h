#include<iostream>
#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>

static const std::string task_member[] = {"name","description","nodes","command"};

struct StTask
{
	std::string name;
	std::string description;
	std::string nodes;
	std::string cmd;
};

class CXMLParser
{
public:
	std::vector<StTask> parse_XML(const std::string& xml_file);
	bool check_format(const std::string& xmlFile);
	int shell_prcss_num(const std::string& xmlFile);
private:
	boost::property_tree::ptree pt;
	boost::property_tree::ptree root;
	StTask task;
};

std::vector<StTask> CXMLParser::parse_XML(const std::string& xml_file)
{
	std::vector<StTask> rslt_tasks;
	boost::property_tree::read_xml(xml_file, pt);
	root = pt.get_child("job");

	for(boost::property_tree::ptree::iterator it = root.begin(); it != root.end(); it++)
	{
		if(it->first == "shell_process")
		{
			boost::property_tree::ptree pt;
			std::string val;
			boost::property_tree::ptree shell_process = it->second;

			for(int i=0; i<4; i++)
			{
				pt = shell_process.get_child(task_member[i]);
				val = pt.data();

				switch(i)
				{
					case 0:
						task.name = val;
						break;
					case 1:
						task.description = val;
						break;
					case 2:
						task.nodes = val;
						break;
					case 3:
						task.cmd = val;
						break;
				}
			}

			rslt_tasks.push_back(task);
		}
	}
	return rslt_tasks;
}

/**
 * xml file format checking depend on a standard.xml file
 */
bool CXMLParser::check_format(const std::string& xml_file)
{
	boost::property_tree::ptree std_pt;
	boost::property_tree::ptree std_pt_root;

	boost::property_tree::read_xml(xml_file, pt);
	root = pt.get_child("job");

	boost::property_tree::read_xml("standard.xml", std_pt);
	std_pt_root = std_pt.get_child("job");

	boost::property_tree::ptree::iterator it = root.begin();
	boost::property_tree::ptree::iterator std_it = std_pt_root.begin();

	while(it != root.end())
	{
		if(it->first != "shell_process")
		{
			if(it->first != std_it->first)
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
	if(std_it != std_pt_root.end())
		return false;
	return true;
}

/**
 * count the number of "shell_process" node(s)
 */
int CXMLParser::shell_prcss_num(const std::string& xml_file)
{
	boost::property_tree::read_xml(xml_file, pt)	;
	root = pt.get_child("job");

	int dscrptn_count = 0;

	boost::property_tree::ptree::iterator it = root.begin();

	while(it != root.end())
	{
		if(it->first == "shell_process")
		{
			dscrptn_count++;
		}
		it++;
	}
	return dscrptn_count;
}
