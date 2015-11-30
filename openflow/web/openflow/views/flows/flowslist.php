<?php
    $this->load->view("common/side.php");
?>
	<div class="m-sidebar-collapsed">
		<ul class="nav nav-pills">

		</ul>

		<div class="arrow-border">
			<div class="arrow-inner"></div>
		</div>
	</div>
	<div class="main-container">
		<div class="container-fluid">
			<section>
				<div class="page-header">
					<h1>流程模板信息</h1>
                    <button type="button" class="btn btn-primary">新建流程</button>
				</div>

				<table class="table table-striped table-condensed">
					<thead>
						<tr>
							<th>Id</th>
							<th>Category</th>
							<th>Name</th>
							<th class="tc">Creater</th>
							<th>Description</th>
							<th>Uptime</th>
						</tr>
					</thead>
					<tbody>
                    <?php
                    foreach ($flowslist as $item):
                    // var_dump($item);
                    $tr_item = "<tr><td>".$item["Id"]."</td><td>".$item["Category"]."</td>";
                    $tr_item = $tr_item."<td><a href='/flows/show?id=".$item["Id"]."' target='_blank'>".$item["Name"]."</a></td>";
                    $tr_item = $tr_item."<td>".$item["Creater"]."</td><td>".$item["Description"]."</td><td>".$item["Uptime"]."</td>";
                    $tr_item = $tr_item."<td class='tr'><a href='#' class='btn'><i class='icon-remove'></i></a>";
                    $tr_item = $tr_item."<a href='#' class='btn'><i class='icon-ok'></i></a></td>";
                    echo $tr_item;
                    endforeach;
                    ?>
					</tbody>
					<tfoot>
						<tr>
							<td colspan="6">
								<div class="pagination">
									<ul>
										<li><a href="#">&laquo;</a></li>
										<li><a href="#">1</a></li>
										<li class="active"><a href="#">2</a></li>
										<li class="disabled"><a href="#">...</a></li>
										<li><a href="#">6</a></li>
										<li><a href="#">7</a></li>
										<li><a href="#">&raquo;</a></li>
									</ul>
								</div>
							</td>
						</tr>
					</tfoot>
				</table>
			</section>
		</div>
	</div>
</body>
</html>
