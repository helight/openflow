<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Instance extends CI_Controller {
    function __construct()
    {
        parent::__construct();
        $this->load->model("InstanceManager");
    }
    public function index()
    {
        $data["mode"] = 2;
        $data["flowslist"] = $this->JobManager->get_jobs_info();
        $this->load->view('flows/flowslist', $data);
    }

	public function show()
	{
        // ["mode"];//=1表示实例化,2表示查看,4=组件开发
        $data["mode"] = 2;
		$this->load->view('flows/edit', $data);
	}

    public function search()
    {
        // data={"State":1,"Message":err.toString()};
         $data["State"] = 0;
         $data["Data"] = 1;
         $tbname = "tbInstance a left join tbTemplate c on c.Id=a.TemplateId";
         $fields = "a.Id,a.Name,a.Description,a.TemplateId, c.Name as TemplateName, a.Uuid, a.State,a.Flag, a.Creater, a.Uptime";
         $where = "1=1";
         $order = 'order by a.Uptime desc,a.State';
         $page = 0;

         $ret = $this->InstanceManager->search($tbname, $fields, $where, $page, $order);
         $data["Data"] = $ret;

         echo json_encode($data);
    }

}
