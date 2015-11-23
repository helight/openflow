<?php
// require_once "base_model.php"
require_once "base_model.php";
// class job_manager extends CI_Model
class job_manager extends base_model
{
    public function job_manager()
    {
        parent::__construct();
    }

    public function get_jobs_info()
    {
        $sql = "select job_id, job_name, xml_desc, time from tbjobs;";
        $query = $this->db->query($sql);
        return $query->fetchArray();
    }
}

?>
