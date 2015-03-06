<?php
    $this->load->view("admin/side.php");
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
						<h1>任务信息 <small>控制面板</small></h1>
					</div>

					<div class="row-fluid">
						<div class="span12 m-widget">
							<div class="m-widget-header">
							<h3>任务状态</h3>
						</div>

							<div class="m-widget-body">
							<div class="row-fluid">
								<a href="#" class="span3 m-stats-item">
									<span class="m-stats-val">200+</span>
									正在执行
								</a>

								<a href="#" class="span3 m-stats-item">
									<span class="m-stats-val">50</span>
									执行完成
								</a>

								<a href="#" class="span3 m-stats-item">
									<span class="m-stats-val">100+</span>
									执行失败
								</a href="#">

								<a href="#" class="span3 m-stats-item">
									<span class="m-stats-val">1000</span>
									执行成功率
								</a href="#">
							</div>
							</div>
					</div>

					<section>
							<div class="row-fluid">
									<div class="span12 m-widget">
										<div class="m-widget-header">
											<h3>任务信息</h3>
										</div>
										<table class="table table-striped table-condensed">
					<thead>
						<tr>
							<th colspan="5">
								<form class="form-inline" />
									<label>Sort by: </label>
									<select>
										<option />Ip
										<option />状态
									</select>
									<label>Order by:</label>
									<select>
										<option />Desc
										<option />Asc
									</select>
								</form>
							</th>
						</tr>
						<tr>
							<th>#</th>
							<th>Ip地址</th>
							<th>物理信息</th>
							<th class="tc">任务数</th>
							<th>状态</th>
							<th></th>
						</tr>
					</thead>
					<tbody>
						<tr>
							<td>001</td>
							<td>192.168.01</td>
							<td>内存,CPU</td>
							<td class="tc">19</td>
							<td><span class="label label-important">失败</span></td>
							<td class="tr">
								<a href="#" class="btn"><i class="icon-remove"></i></a>
								<a href="#" class="btn"><i class="icon-ok"></i></a>
							</td>
						</tr>

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

									</div>
							</div>
					</section>

	</section>
	</div>
</body>
</html>
