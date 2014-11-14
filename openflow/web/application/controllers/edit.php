<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Edit extends CI_Controller {

	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/index.php/main
	 *	- or -
	 * 		http://example.com/index.php/main/index
	 *	- or -
	 * Since this controller is set as the default controller in
	 * config/routes.php, it's displayed at http://example.com/
	 *
	 * So any other public methods not prefixed with an underscore will
	 * map to /index.php/welcome/<method_name>
	 * @see http://codeigniter.com/user_guide/general/urls.html
	 */
	public function show()
	{
        // ["mode"];//=1表示实例化,2表示查看,4=组件开发
        $data["mode"] = 2;
		$this->load->view('editor', $data);
	}

    public function edit_flow()
    {
        $data["mode"] = 4;
		$this->load->view('editor', $data);
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
