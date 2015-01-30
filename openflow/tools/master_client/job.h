#ifndef JOB_H
#define JOB_H
#pragma once
#include <iostream>
#include "rpc/master/MasterService.h"

namespace openflow { namespace tools{ namespace master_client {
class CJob
{
    public:
        void set_xml(const std::string &filename);
        /**
         * Func: save xml to database, then return job id.
         * db_name: database name.
         * info: some of job information.
         *  job_id is a outer value. xml_desc is a input value.
         * return: sucess with job id, otherwise -1.
         */
        int store(const std::string & db_name, openflow::job_info &info);
	bool report_state();

    private:
        std::string _name; //job name
        int _id; //job id
        std::string _xml; //xml name
};

}}} //namespace openflow::tools::master_client
#endif // JOB_H
