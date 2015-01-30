<?php

require_once "ThriftBase.php";

class MasterOpt extends ThriftBase
{
	public function __construct()
	{
		parent::__construct();
	}
	public function __destruct()
	{
		parent::__destruct();
	}
	public function submit_job($jobid)
	{
		$client = $this->init("127.0.0.1",9080);
		$result = "";
		if($this->is_init)
		{
			$result = $client->submit_job($jobid);
		}
		$this->close();
		return $result;
	}
	public function execute_jobinfo($jobid)
	{
		$client = $this->init("127.0.0.1",9080);
		$result = "";
		if($this->is_init)
		{
			$result = $client->execute_jobinfo();
		}
		$this->close();
		return $result;
	}
}
