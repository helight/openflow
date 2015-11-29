<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Flows extends CI_Controller {
    function __construct()
    {
        parent::__construct();
        $this->load->model("JobManager");
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

    public function edit_flow()
    {
        $data["mode"] = 4;
		$this->load->view('flows/edit', $data);
    }

    public function test()
    {
        var_dump($this->JobManager->get_jobs_info());
        $this->load->view("blogview");
    }

    public function component()
    {
        if (isset($_GET['cmd']))
        {
            $ret = 0;
            $cmd = $_GET['cmd'];
            // echo "cmd: ".$cmd."<br>";
            switch ($cmd)
            {
            case 'getdraft':
            case 'get':
                if (isset($_GET['id']))
                {
                    $data["State"] = 0;
                    $ret = $this->JobManager->get_job_info($_GET['id']);
                    if (count($ret) > 0)
                    {
                        $data["Data"] = $ret[0];
                        echo json_encode($data);
                    }
                }
                break;
            default:
            break;
            }
        }
    }

    public function template()
    {
        $data["State"] = 0;
        if (isset($_GET['cmd']))
        {
            $ret = 0;
            $cmd = $_GET['cmd'];
            // echo "cmd: ".$cmd."<br>";
            switch ($cmd)
            {
            case "insert":
                if (isset($_POST['EditorXml']))
                {
                    $data["Data"] = $this->JobManager->add_jobs($_POST['Name'], 'helightxu', $_POST['EditorXml']
                        , $_POST['TemplateXml'], $_POST['Description']);
                }
                break;
            case "update":
                $data["Data"] = $this->JobManager->update_job($_POST['Id'], $_POST['EditorXml'], $_POST['TemplateXml']
                                                      , $_POST['Name'], $_POST['Description']);
                break;
            case "savedraft":
                break;
            case "inserttemplate":
                break;
            case "updatetemplate":
                break;
            case "updatepermission":
                break;
            default:
                break;
            }
            echo json_encode($data);
        }
    }
}
