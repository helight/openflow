<?php
class InstanceManager extends CI_Model
{
    function __construct()
    {
        parent::__construct();
        $this->db = $this->load->database('default', TRUE);
    }

    public function search($tbname, $fields, $where, $page, $order)
    {
        $sql = "select ".$fields." from ".$tbname." where ".$where." ".$order;
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
