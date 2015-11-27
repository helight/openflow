<?php
// class job_manager extends CI_Model
class JobManager extends CI_Model
{
    var $job_category = 0;
    var $job_name = "";
    var $creater = "";
    var $flow_xml = "";
    var $desc = "";
    var $uptime = "";

    function __construct()
    {
        parent::__construct();
        $this->db = $this->load->database('default', TRUE);
    }

    public function get_jobs_info()
    {
        $sql = "select id, job_name, creater, flow_xml, desc, uptime from tbjobs;";
        $query = $this->db->query($sql);
        return $query->result_array();
    }

    public function get_job_info($id)
    {
        $sql = "select id, job_name as Name, creater, flow_xml as EditorXml, desc, uptime from tbjobs where id=".$id.";";
        $query = $this->db->query($sql);
        return $query->result();
    }

    public function add_jobs($job_name, $creater, $xml_info, $desc, $uptime)
    {
        $this->job_name = $job_name;
        $this->creater = $creater;
        $this->flow_xml = $xml_info;
        $this->desc = $desc;
        $this->uptime  = $uptime;

        $this->db->insert('tbJobs', $this);
        return $this->db->insert_id();
    }
}

?>
