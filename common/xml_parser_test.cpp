// Copyright (c) 2014, OpenFlow
// Author:CptZhao<zhaonanyu@gmail.com>
// Created: 2014-08-30
// Description:
//

#include <vector>
#include <gtest/gtest.h>
#include "xml_parser.h"

TEST(CXMLParser,parserXML)
{
	std::vector<StTask> std_tasks;
	std::vector<StTask>::iterator std_it;

	std::vector<StTask> tasks;
	std::vector<StTask>::iterator it;
	
	StTask task;

	task.name = "test node1";
	task.description = "node1";
	task.nodes = "127.0.0.1";
	task.cmd = "#!/bin/sh";
	std_tasks.push_back(task);

	task.name = "test node2";
	task.description = "node2";
	task.nodes = "127.0.071";
	task.cmd = "#!/bin/sh.distrib";
	std_tasks.push_back(task);

	task.name = "test node3";
	task.description = "node3";
	task.nodes = "127.0.0.2";
	task.cmd = "#cd /usr/share";
	std_tasks.push_back(task);

	
	CXMLParser parser;
	tasks = parser.parse_XML("test.xml");

	std_it = std_tasks.begin();
	it = tasks.begin();

	while(std_it != std_tasks.end() && it != tasks.end())
	{   
	    EXPECT_STREQ(std_it->name.c_str(),it->name.c_str());
		EXPECT_STREQ(std_it->description.c_str(),it->description.c_str());
		EXPECT_STREQ(std_it->nodes.c_str(),it->nodes.c_str());
		EXPECT_STREQ(std_it->cmd.c_str(),it->cmd.c_str());
		std_it++;
		it++;
	}
}

TEST(CXMLParser,check_format)
{
	CXMLParser parser;
	bool rslt = parser.check_format("test.xml");
	EXPECT_TRUE(rslt);
}

TEST(CXMLParser,shell_prcss_num)
{
	CXMLParser parser;
	EXPECT_EQ(3,parser.shell_prcss_num("test.xml"));
}

int main(int argc,char** argv)
{
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
