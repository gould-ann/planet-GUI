<?php
	
	// php_get_handler
	// gets the last 10 lines from the history of roomba output (from the roomba to this machine)
	$output = shell_exec('tail -n 10 roomba_history.txt');
	echo $output;
?> 
