<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Flows extends CI_Controller {

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

    public function save_xml()
    {
        echo "asdfasdf<br>";
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
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
