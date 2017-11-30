<?php
	
	// echo ":)";
	$output = shell_exec('tail roomba_output.txt 10');
	echo "<br>" . $output;
?> 
