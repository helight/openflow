// Copyright (c) 2014,ZhaoStudio
// Author:CptZhao<zhaonanyu@gmail.com>
// Created: 2014-08-13
// Description:
//
#include"xml_parser.h"
#include<vector>

/*
struct StTask
{
	std::string name;
	std::string description;
	std::string nodes;
	std::string cmd;
}
*/

void print_rslt(vector<StTask> tasks)
{
	vector<StTask>::iterator it;

	string val;
	int count=0;

	for(it=tasks.begin();it!=tasks.end();it++)
	{
		val=it->name;
		cout<<val<<endl;


		cout<<(it->name)<<endl;
		cout<<(it->description)<<endl;
		cout<<(it->nodes)<<endl;
		cout<<(it->cmd)<<endl;
	}


}

int main()
{
	vector<StTask> tasks;
	xmlParser Cxml_parser;
	bool Ck_rslt=Cxml_parser.format_Ck("test.xml");
	if(Ck_rslt)
		cout<<"format correct"<<endl;
	else
	{
		cout<<"file format incorrect, plz check again"<<endl;
		return 0;
	}
	tasks=Cxml_parser.parseXML("test.xml");
	cout<<"printing"<<endl;
	print_rslt(tasks);
	cout<<"print complete"<<endl;
	return 1;
}

