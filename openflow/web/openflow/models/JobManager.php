<?php
// class job_manager extends CI_Model
class JobManager extends CI_Model
{
    var $Category = 0;
    var $Name = "";
    var $Creater = "";
    var $EditorXml= "";
    var $TemplateXml = "";
    var $Description = "";
    var $Uptime = "";

    function __construct()
    {
        parent::__construct();
        $this->db = $this->load->database('default', TRUE);
    }

    public function get_jobs_info()
    {
        $sql = "select Id, Category, Name, Creater, EditorXml, TemplateXml, Description, Uptime from tbjobs;";
        $query = $this->db->query($sql);
        return $query->result_array();
    }

    public function get_job_info($id)
    {
        $sql = "select Id, Name, Creater, EditorXml, TemplateXml, Description, Uptime from tbjobs where id=".$id.";";
        $query = $this->db->query($sql);
        return $query->result();
    }

    public function add_jobs($name, $creater, $edit_xml, $tmp_xml, $desc)
    {
        $this->Name = $name;
        $this->Creater = $creater;
        $this->EditorXml = $edit_xml;
        $this->TemplateXml = $tmp_xml;
        $this->Description = $desc;
        $this->Uptime  = date("Y-m-d H:i:s", time());

        $this->db->insert('tbJobs', $this);
        return $this->db->insert_id();
    }

    public function update_job($Id, $edit_xml, $tmp_xml, $name, $desc)
    {
        $this->Name = $name;
        $this->EditorXml = $edit_xml;
        $this->TemplateXml = $tmp_xml;
        $this->Description = $desc;
        $this->Uptime  = date("Y-m-d H:i:s", time());
        $this->db->update('tbJobs', $this, array('Id' => $_POST['Id']));
        return $_POST['Id'];
    }
}

?>
