<?php

$GLOBALS['THRIFT_ROOT'] = APPPATH.'thirdlib/thrift_php/';

require_once $GLOBALS['THRIFT_ROOT'].'Thrift/ClassLoader/ThriftClassLoader.php';

use Thrift\ClassLoader\ThriftClassLoader;
use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\TSocket;
use Thrift\Transport\TSocketPool;
use Thrift\Transport\THttpClient;
use Thrift\Transport\TBufferedTransport;
use Thrift\Exception\TException;
use Thrift\Transport\TFramedTransport;
use openflow\master\MasterServiceClient;
class ThriftBase
{
	public function __construct()
	{
		$this->is_init = false;
		$this->transport = NULL;
	}
	public function __destruct()
	{
		if($this->is_init)
		{
			$this->transport->close();
		}
	}
	public function init($host = "127.0.0.1",$port = 8090)
	{
		if(false == $this->is_init){
			$loader = new ThriftClassLoader();
                	$loader->registerNamespace('Thrift',$GLOBALS['THRIFT_ROOT']);
             		$loader->registerDefinition('openflow\master',APPPATH.'thirdlib/');
                	$loader->register();
                	$socket = new TSocket($host,$port);
               		$framedSocket = new TFramedTransport($socket);
                	$this->transport = $framedSocket;
                	$protocol = new TBinaryProtocol($this->transport);
                	$this->transport->open();
                	$client= new MasterServiceClient($protocol);
        		$this->is_init = true;
			return $client;
		}
	}
	public function close()
	{
		if($this->is_init)
		{
			$this->transport->close();
		}
	}
}
