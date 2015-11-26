<?php

class Main extends CI_Controller {

	public function __construct()
	{
		parent::__construct();
	}

	public function index()
	{
		$this->admin();
	}

    public function admin()
    {
        // echo APPPATH;
		$this->load->view('main');
    }
}
