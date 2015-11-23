<!doctype html>
<html>
<head>
	<title>OpenFlow</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta name="viewport" content="initial-scale=1,maximum-scale=1,user-scalable=no" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/bootstrap.css" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/bootstrap-responsive.css" />
	<link rel="stylesheet" type="text/css" href="../content/css/admin/style.css" />

	<script type='text/javascript' src='../content/js/admin/jsapi.js'></script>
	<script type="text/javascript" src="../content/js/js/jquery.min.js"></script>
	<script type="text/javascript" src="../content/js/admin/main.js"></script>
	<script type="text/javascript" src="../content/js/admin/bootstrap.min.js"></script>
</head>

<body>
	<div class="navbar navbar-fixed-top m-header">
		<div class="navbar-inner m-inner">
			<div class="container-fluid">
				<a class="brand m-brand" href="admin">Openflow</a>

	            <form class="form-search m-search span5" />
					<input type="text" placeholder="Type to search" class="span5 search-query" />
				</form>

				<button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
		            <span class="icon-bar"></span>
		            <span class="icon-bar"></span>
		            <span class="icon-bar"></span>
		        </button>

				<div class="nav-collapse collapse">

					<div class="btn-group pull-right">
				        <a class="btn dropdown-toggle" data-toggle="dropdown" href="#">
			          		<i class="icon-user"></i> openflow user
			          		<span class="caret"></span>
				        </a>
				        <ul class="dropdown-menu">
			          		<li><a href="#"><i class="icon-user"></i>用户信息</a></li>
			          		<li><a href="#"><i class="icon-cog"></i>个人设置</a></li>
	 		 				<li class="divider"></li>
			          		<li><a href="login.html"><i class="icon-off"></i>注销</a></li>
				        </ul>
			      	</div>
	          	</div>
			</div>
		</div>
	</div>
	<div class="m-top"></div>
	<aside class="sidebar">
		<ul class="nav nav-tabs nav-stacked">
			<li class="active">
				<a href="/">
					<div>
						<div class="ico">
							<img src="../content/img/ico/ico-home.png" />
						</div>
						<div class="title">
							主页
						</div>
					</div>

					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/flows">
					<div>
						<div class="ico">
						<img src="../content/img/ico/ico-more.png" />
						</div>
						<div class="title">
							流程信息
						</div>
					</div>

					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/master">
					<div>
						<div class="ico">
						<img src="../content/img/ico/ico-report.gif" />
						</div>
						<div class="title">
							Master信息
						</div>
					</div>

					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/agent">
					<div>
						<div class="ico">
						<img src="../content/img/ico/ico-report.gif" />
						</div>
						<div class="title">
							agent信息
						</div>
					</div>

					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/task">
					<div>
						<div class="ico">
							<img src="../content/img/ico/ico-setting.gif" />
						</div>
						<div class="title">
							任务信息
						</div>
					</div>
					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>


			<li class="">
				<a href="/admin/setting">
					<div>
						<div class="ico">
						<img src="../content/img/ico/ico-setting.gif" />
						</div>
						<div class="title">
							设置
						</div>
					</div>
					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/admin/user">
					<div>
						<div class="ico">
							<img src="../content/img/ico/ico-user.gif" />
						</div>
						<div class="title">
							用户管理
						</div>
					</div>
					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>

			<li class="">
				<a href="/admin/help">
					<div>
						<div class="ico">
							<img src="../content/img/ico/ico-help.gif" />
						</div>
						<div class="title">
							帮助
						</div>
					</div>
					<div class="arrow">
						<div class="bubble-arrow-border"></div>
						<div class="bubble-arrow"></div>
					</div>
				</a>
			</li>
	    </ul>
	</aside>
