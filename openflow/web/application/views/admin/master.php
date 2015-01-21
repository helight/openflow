<?php
	require_once("side.php");
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
					<h1>Master控制面板</h1>
				</div>
				<div class="row-fluid">

					<div class="alert alert-info">
						<button class="close" data-dismiss="alert">x</button>
						<strong>警告信息!</strong> 实时打印一些警告信息
						<a href="#">
							<span class="label label-important">处理级别!</span>	
						</a>
					</div>

					<div class="alert">
						<button class="close" data-dismiss="alert">x</button>
						<strong>警告信息!</strong> 实时打印一些警告信息 
						<a href="#">
							<span class="label label-info">处理级别</span>	
						</a>
					</div>
				</div>
				<div class="row-fluid">
					<div class="span4 m-widget">
						<div class="m-widget-header">
							<h3>Master物理信息</h3>
						</div>
						<div class="m-widget-body">
							<table class="table table-striped table-condensed">
								<thead>
									<tr>
										<th></th>
										<th></th>
										<th>&nbsp;</th>
									</tr>
								</thead>
								<tbody>
									<tr>
										<td>内存使用率</td>
										<td><a href="#">Order item 1</a></td>
										<td class="tr">
											<a href="#" class="btn btn-primary">View</a>
										</td>
									</tr>
									<tr>
										<td>磁盘使用率</td>
										<td><a href="#">Order item 2</a></td>
										<td class="tr">
											<a href="#" class="btn btn-primary">View</a>
										</td>
									</tr>
									<tr>
										<td>CPU使用率</td>
										<td><a href="#">Order item 3</a></td>
										<td class="tr">
											<a href="#" class="btn btn-primary">View</a>
										</td>
									</tr>
								</tbody>
							</table>
						</div>
					</div>

					<div class="span8 m-widget">
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
				</div>
						</div>
					</div>
				</div>
			</section>
		</div>
	</div>
</body>
</html>