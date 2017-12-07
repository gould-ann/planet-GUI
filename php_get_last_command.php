<?php
	
	// php_get_last_command
	// prints off the last output sent back from the roomba to this machine
	// removes that line from the file storing it

	$output = shell_exec("tail -n 1 roomba_output.txt;sed -i '' '$ d' roomba_output.txt");
	echo $output;
?> 
