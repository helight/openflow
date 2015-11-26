<?php
class Master extends CI_Controller {

	public function __construct()
	{
		parent::__construct();
        $this->load->model("JobManager");
	}

	public function index()
	{
		$this->load->view('admin/master');
	}

    public function get_jobs()
    {
        echo json_encode($this->JobManager->get_jobs_info());
    }
}
