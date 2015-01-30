// Define maximum available item on side bar
var _windowInnerHeight = 0;
var _mainContainerHeight = 0;

$(document).ready(function(){

	// Init upload modal
	$('#myModal').modal({
		show: false
	});

	// Auto hide more button if sidebar is not full
	$('#btn-more').hide();

	// Apply bootstrap button plugin
	$('.btn').button();

	// Apply bootstrap tooltip plugin
	$('.bar').tooltip('show')

	// Handle view more button for left sidebar
	$('#btn-more').bind('click', function(e){
		e.preventDefault();

		var _this = $(this);

		if($('.m-sidebar-collapsed .nav li').length == 0) {
			return false;
		}

		if(_this.data('isShow') != undefined && _this.data('isShow')) {
			_this.data({isShow: false});
			$('.m-sidebar-collapsed').fadeOut(); return false;
		}

		_this.data({isShow: true});

		var _offset = $(this).find('img').offset();

		$('.m-sidebar-collapsed').css({
			'top': _offset.top - 15 - window.scrollY + 'px',
			'left': _offset.left + 60 + 'px'
		}).fadeIn();
	});

	// get window inner height
	_windowInnerHeight = $(window).innerHeight();
	// get init main container height
	_mainContainerHeight = $('.main-container').innerHeight();

	function updateMainContainerHeight() {
		// Try to update main container height with available browser height
		if($('.sidebar').innerHeight() > _mainContainerHeight) {
			$('.main-container').css({
				height: $('.sidebar').innerHeight()
			});
		} else {
			$('.main-container').css({
				height: 'auto'
			});
		}
	}

	$(window).bind('resize', function(e){

		updateMainContainerHeight();

		$('.m-sidebar-collapsed').fadeOut();
		$('#btn-more').data({isShow: false});

		
		if($(window).innerWidth() <= 768) {
			pushSideBarItem($('.m-sidebar-collapsed .nav li').length);
			return false;
		}

		var _itemsCount = ($(window).innerHeight() - 60) / 76;
			_itemsCount = parseInt(_itemsCount, 10);

		// Get current sidebar length
		var _currSideBarLength = $('.sidebar .nav li').length;

		if(_itemsCount < 2 ) {
			_itemsCount = 2;
		}

		if(_currSideBarLength == _itemsCount) {
			return false;
		}


		if(_currSideBarLength > _itemsCount) {
			// Pop out
			popSideBarItem(_currSideBarLength - _itemsCount);
		} else {
			// Push in
			pushSideBarItem(_itemsCount - _currSideBarLength);
		}

		$('#btn-more').toggle($('.m-sidebar-collapsed .nav li').length > 0);

		if(_currSideBarLength == _itemsCount) {
			return false;
		}
	}).trigger('resize');

	function popSideBarItem(items) {

		_currSideBar = $('.sidebar .nav li');

		for(var i = _currSideBar.length - 2, iCount = 0; iCount < items; i--,iCount++){
			$('.m-sidebar-collapsed .nav').prepend(_currSideBar[i]);
		}
	}

	function pushSideBarItem(items) {
		_collapseSideBar = $('.m-sidebar-collapsed .nav li');

		for(var i = 0, iCount = 0; iCount < items; i++, iCount++) {
			$('#btn-more').parents('li').before(_collapseSideBar[i]);
		}
	}
});