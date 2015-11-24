<?php
// require_once "base_model.php"
require_once "base_model.php";
// class job_manager extends CI_Model
class job_manager extends base_model
{
    var $job_name = "";
    var $creater = "";
    var $flow_xml = "";
    var $desc = "";
    var $uptime = "";
    public function job_manager()
    {
        parent::__construct();
    }

    public function get_jobs_info()
    {
        $sql = "select job_id, job_name, creater, flow_xml, desc, uptime from tbjobs;";
        $query = $this->db->query($sql);
        return $query->fetchArray();
    }

    public function add_jobs($job_name, $creater, $xml_info, $desc, $uptime)
    {
        $this->job_name = $job_name;
        $this->creater = $creater;
        $this->flow_xml = $xml_info;
        $this->desc = $desc;
        $this->uptime  = $uptime;

        $this->db->insert('tbJobs', $this);
    }
}

?>
