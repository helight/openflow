<?php
echo "asdfasdfasdf";
// phpinfo();
// $db = sqlite_open("database/openflow.db"); //打开db.sqlite数据库，如果不存在则尝试创建。
// $result = sqlite_query($db, "select * from tbJobs");
// var_dump($result);

$db = new SQLite3('database/openflow.db');
$result = $db->query("select * from tbJobs");
var_dump($result->fetchArray());
?>
