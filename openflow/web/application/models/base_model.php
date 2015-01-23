<?php
class base_model extends CI_Model
{
    public $db;
    public function base_model()
    {
        parent::__construct();
        // $path = system("pwd");
        $path =  getcwd();
        // echo $path;
        $this->db = new SQLite3($path.'/database/openflow.db');
    }
}
?>
