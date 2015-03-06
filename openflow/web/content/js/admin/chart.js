google.load('visualization', '1', {'packages': ['geochart', 'corechart']});
google.setOnLoadCallback(drawChart);

$(document).ready(function(){
	$('.btn-chart').bind('click', function(){
		$('#line-chart').empty();
		drawLineChart($(this).attr('data'));
	});
});

function drawChart() {
	drawRegionsMap();
	drawLineChart();
}

function drawRegionsMap() {
    var data = google.visualization.arrayToDataTable([
      ['Country', 'Popularity'],
      ['Germany', 200],
      ['United States', 300],
      ['Brazil', 400],
      ['Canada', 500],
      ['France', 600],
      ['RU', 700]
    ]);

    var options = {};

    var chart = new google.visualization.GeoChart(document.getElementById('geo-chart'));
    chart.draw(data, options);
};

function drawLineChart(chartType) {

	if(chartType == undefined) {
		chartType = 'data1';
	}

	var data = {
		data1: [
			['Year', 'Sales', 'Expenses'],
			['2004',  1000,      400],
			['2005',  1170,      460],
			['2006',  660,       1120],
			['2007',  1030,      540]
		],
		data2: [
			['Year', 'Sales', 'Expenses'],
			['2004',  1000,      400],
			['2005',  2500,      300],
			['2006',  800,       750],
			['2007',  500,      200]
		],
		data3: [
			['Year', 'Sales', 'Expenses'],
			['2004',  1500,      459],
			['2005',  560,      200],
			['2006',  780,       560],
			['2007',  1500,      2000]
		]
	};

	var options = {
	  title: 'Company Performance'
	};

	var chart = new google.visualization.LineChart(document.getElementById('line-chart'));

	var chartData = [];

	switch(chartType)
	{
		case 'data1':
			chartData = data.data1;
			break;
		case 'data2':
			chartData = data.data2;
			break;
		case 'data3':
			chartData = data.data3;
			break;
	}
	if(google) {
		chartData = google.visualization.arrayToDataTable(chartData);
		chart.draw(chartData, options);
	}
}