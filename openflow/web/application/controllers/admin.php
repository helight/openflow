<?php

class Admin extends CI_Controller {

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
	public function __construct()
	{
		parent::__construct();
	}

	public function index()
	{
        echo $APPPATH;
		$this->load->view('admin/admin');
	}

    /// for logging
    public function login()
    {
        $this->load->view('admin/login');
    }

    public function login_post()
    {
        $username = $_POST['username'];
        $pass = $_POST['pass'];
        echo $pass.$username;
    }

    /// for setting
    public function setting()
    {
        $this->load->view('admin/setting');
    }

    /// for user
    public function user()
    {
        $this->load->view('admin/user');
    }

    /// for help
    public function help()
    {
		$this->load->view('admin/help');
    }

}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */
