/*
 * =====================================================================================
 *
 *       Filename:  sys_info.cc
 *        Version:  1.0
 *        Created:  2014年11月24日 14时28分39秒
 *       Revision:  none
 *       Compiler:  g++
 *         Author:  zomoe, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/param.h>		/* for HZ */
#include <sys/timerfd.h>
#include <time.h>

/*
 *  This file defines the interface which could get the systeminfo etc..
 */


struct statics
{
    char **procstate_names;
    char **cpustate_names;
    char **memory_names;
#ifdef ORDER
    char **order_names; /* for extra use*/
#endif
};

/*
 * the system_info struct is filled in by a machine dependent routine.
 */

#define NCPUSTATES 4

#define NMEMSTATS 6

#define LEN 512

/* these are for calculating cpu state percentages */

static int cpu_states[NCPUSTATES];
static int memory_stats[NMEMSTATS];
static long cp_time[NCPUSTATES];
static long cp_old[NCPUSTATES];
static long cp_diff[NCPUSTATES];

struct system_info
{
    double load_avg[3];
    int    p_total;
    int    *cpustates;
    int    *memory;
};

static inline char *
skip_ws(const char *p)
{
    while (isspace(*p)) p++;
    return (char *)p;
}

static inline char *
skip_token(const char *p)
{
    while (isspace(*p)) p++;
    while (*p && !isspace(*p)) p++;
    return (char *)p;
}


long 
percentages(int cnt,int *out,\
			register long *new_t,
			register long *old,
			long *diffs)
{
    register int i;
    register long change;
    register long total_change;
    register long *dp;
    long half_total;

    /* initialization */
    total_change = 0;
    dp = diffs;

    /* calculate changes for each state and the overall change */
    for (i = 0; i < cnt; i++)
    {
	if ((change = *new_t - *old) < 0)
	{
	    /* this only happens when the counter wraps */
	    change = (int)
		((unsigned long)*new_t-(unsigned long)*old);
	}
	total_change += (*dp++ = change);
	*old++ = *new_t++;
    }

    /* avoid divide by zero potential */
    if (total_change == 0)
    {
	total_change = 1;
    }

    /* calculate percentages based on overall change, rounding up */
    half_total = total_change / 2l;
    for (i = 0; i < cnt; i++)
    {
	*out++ = (int)((*diffs++ * 1000 + half_total) / total_change);
    }

    /* return the total in case the caller wants to use it */
    return(total_change);
}

void
get_system_info( struct system_info *info )
    
{
    char buffer[4096+1];
    int fd, len;
    char *p;
    int i;

    /* get load averages */
    {
	fd = open("/proc/loadavg", O_RDONLY);
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);
	buffer[len] = '\0';

	info->load_avg[0] = strtod(buffer, &p);
	info->load_avg[1] = strtod(p, &p);
	info->load_avg[2] = strtod(p, &p);
    }

    /* get the cpu time info */
    {
	fd = open("/proc/stat", O_RDONLY);
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);
	buffer[len] = '\0';

	p = skip_token(buffer);			/* "cpu" */
	cp_time[0] = strtoul(p, &p, 0);	
	cp_time[1] = strtoul(p, &p, 0);
	cp_time[2] = strtoul(p, &p, 0);
	cp_time[3] = strtoul(p, &p, 0);

	/* convert cp_time counts to percentages */
	percentages(4, cpu_states, cp_time, cp_old, cp_diff);
    }
	
    /* get system wide memory usage */
    {
	char *p;

	fd = open("/proc/meminfo", O_RDONLY);
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);
	buffer[len] = '\0';

	/* be prepared for extra columns to appear be seeking
	   to ends of lines */
	
	p = buffer;
	p = skip_token(p);
	memory_stats[0] = strtoul(p, &p, 10); /* total memory */
	
	p = strchr(p, '\n');
	p = skip_token(p);
	memory_stats[1] = strtoul(p, &p, 10); /* free memory */
	
	
	p = strchr(p, '\n');
	p = skip_token(p);
	memory_stats[2] = strtoul(p, &p, 10); /* buffer memory */
	
	p = strchr(p, '\n');
	p = skip_token(p);
	memory_stats[3] = strtoul(p, &p, 10); /* cached memory */
	
	for(i = 0; i< 10 ;i++) {
		p++;
		p = strchr(p, '\n');
	}
	
	p = skip_token(p);
	memory_stats[4] = strtoul(p, &p, 10); /* total swap */
	
	p = strchr(p, '\n');
	p = skip_token(p);
	memory_stats[5] = strtoul(p, &p, 10); /* free swap */
	
    }

    /* set arrays and strings */
    info->cpustates = cpu_states;
    info->memory = memory_stats;
}

std::string 
get_local_ip()  
{  

	int sock_get_ip;  
	char ipaddr[50];  
	std::string ip_str;
	struct   sockaddr_in *sin;  
	struct   ifreq ifr_ip;     

	if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{    
		return "";  
	}  

	memset(&ifr_ip, 0, sizeof(ifr_ip));     
	strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);     

	if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )     
	{     
		return "";     
	}       
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
	strcpy(ipaddr,inet_ntoa(sin->sin_addr));               
	close( sock_get_ip );  
	ip_str.append(ipaddr);
	return ip_str += "\n";  
} 

struct 
tmp_info{
 	 std::string remain_mem;
 	 std::string mem_use_percent;
 	 std::string cpu_idle_percent;
 	 std::string cpu_load;
 	 std::string swap_use_percent;
	 std::string ipaddr;
};

class 
Sys_Info{
public:
	Sys_Info(struct tmp_info &tmp_t){
		char tmp[LEN];
		std::string ipaddr;
		get_system_info(&info);

		snprintf(tmp,LEN,"remain_mem:%d\n",info.memory[1]);
		tmp_t.remain_mem.assign(tmp);

		snprintf(tmp,LEN,"mem_use_percent:%.2f\n",\
					((double)(info.memory[0] - info.memory[1])/info.memory[0]));
		tmp_t.mem_use_percent.assign(tmp);

		snprintf(tmp,LEN,"loadavg: 1min:%.2f | 5min:%.2f | 10min:%.2f\n",\
					info.load_avg[0],info.load_avg[1],info.load_avg[2]);
		tmp_t.cpu_load.assign(tmp);

		snprintf(tmp,LEN,"cpu_idle_percent:%.1f%%\n",(float)info.cpustates[3]/10);
		tmp_t.cpu_idle_percent.assign(tmp);

		snprintf(tmp,LEN,"swap_use_percent:%.2f\n",\
					((double)(info.memory[4] - info.memory[5])/info.memory[4]));
		tmp_t.swap_use_percent.assign(tmp);
		ipaddr += "local_ip:";
		ipaddr += get_local_ip();
		tmp_t.ipaddr.assign(ipaddr);	
	}
	~Sys_Info(){
	}
private:
	struct system_info info;
};
