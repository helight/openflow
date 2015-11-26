<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Flows extends CI_Controller {
    function __construct()
    {
        parent::__construct();
        $this->load->model("JobManager");
    }
    public function index()
    {
        $this->edit_flow();
    }

	public function show()
	{
        // ["mode"];//=1表示实例化,2表示查看,4=组件开发
        $data["mode"] = 2;
		$this->load->view('flows/flows', $data);
	}

    public function edit_flow()
    {
        $data["mode"] = 4;
		$this->load->view('flows/edit', $data);
    }

    public function test()
    {
        var_dump($this->JobManager->get_jobs_info());
    }

    public function save_xml()
    {
        if (isset($_GET['cmd']))
        {
            echo "cmd: ".$_GET['cmd']."<br>";
        }
        if (isset($_POST['EditorXml']))
        {
            echo "EditorXml: ".$_POST['EditorXml']."<br>";
        }
        if (isset($_POST['TemplateXml']))
        {
            echo "TemplateXml: ".$_POST['TemplateXml']."<br>";
        }
        if (isset($_POST['Name']))
        {
            echo "Name: ".$_POST['Name']."<br>";
        }
        if (isset($_POST['Description']))
        {
            echo "Description: ".$_POST['Description']."<br>";
        }
        if (isset($_POST['url']))
        {
            echo "url: ".$_POST['url']."<br>";
        }
    }

    public function template()
    {
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
                    # echo "EditorXml: ".$_POST['EditorXml']."<br>";
                    $sql = "insert into tbJobs(job_name, creater, xml_info, desc, time)VALUES(";
                    $sql = $sql."'".$_POST['Name']."', 'helightxu', '".$_POST['EditorXml'];
                    $sql = $sql."', '".$_POST['Description']."', '".date("Y-m-d H:i:s", time())."');";
                    // echo $sql;
                    $ret = $this->JobManager->add_jobs($_POST['Name'], 'helightxu', $_POST['EditorXml'],
                        $_POST['Description'], date("Y-m-d H:i:s", time()));
                }
                break;
            case "update":
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
            echo $ret;
        }
    }
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
