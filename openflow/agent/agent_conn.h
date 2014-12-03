/*
 * =====================================================================================
 *
 *       Filename:  agent_conn.h
 *        Version:  1.0
 *        Created:  2014年11月27日 16时13分24秒
 *       Revision:  none
 *       Compiler:  g++
 *         Author:  zomoe, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#include <glog/logging.h>
#include <thrift_helper.h>
#include "../config.h"
#include "rpc/master/MasterService.h"

namespace openflow { namespace agent {
	class CagentConn
	{
		public:
			CagentConn(const std::string& host,\
						uint16_t port);
			~CagentConn();
			void SetInfo(const openflow::agent_state & State);
			int32_t report_status(const openflow::agent_state &State);
		private:
			openflow::agent_state state;
			common::CThriftClientHelper<openflow::master::MasterServiceClient> *Master;
	};
}} //end namespace

