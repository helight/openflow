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
		if(!$this->is_init)
		{
			$result = $client->submit_job($jobid);	
			$this->close();
		}
		return $result;
	}
	
	public function execute_jobinfo()
	{
		$client = $this->init("127.0.0.1",9080);
		$result = "";
		if(!$this->is_init)
		{
			$result = $client->execute_joninfo();	
			$this->close();
		}
		return $result;
	}

	public function stop_job($jobid)
	{
		$client = $this->init("127.0.0.1",9080);
		$result = "";
		if(!$this->is_init)
		{
			$result = $client->stop_job($jobid);	
			$this->close();
		}
		return $result;
	}
	
	public function kill_job($jobid)
	{
		$client = $this->init("127.0.0.1",9080);
		$result = "";
		if(!$this->is_init)
		{
			$result = $client->kill_jon($jobid);	
			$this->close();
		}
		return $result;
	}

}
