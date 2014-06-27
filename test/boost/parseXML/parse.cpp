#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>

using namespace std;
using namespace boost::property_tree;

static const string task_member[] = {"name","description","nodes", "command"};

struct StTask
{
    std::string name;
    std::string description;
    std::string nodes;
    std::string cmd;
};

void parseXML(const char *filename, vector<StTask> &tasks)
{
    ptree pt;
    ptree root;
    StTask task;

    read_xml(filename, pt);
    root = pt.get_child("job");

    for(ptree::iterator it = root.begin(); it != root.end(); it++)
    {
        if(it->first == "shell_process")
        {
            ptree pt;
            string val;
            ptree shell_process = it->second;

            int i=0;
            for(; i < 4; i++)
            {
               pt = shell_process.get_child(task_member[i]);
               val = pt.data();
               cout << val << endl;
            }

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

            tasks.push_back(task);
        }
    }
}

int main()
{
    vector<StTask> tasks;

    parseXML("demo.xml", tasks);

    return 0;
}
