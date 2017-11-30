<?php

	// echo $_GET["char"] . " sent";
	$output = shell_exec('echo ' . $_GET["char"] . " > roomba_input.txt");
	echo $output;
?> 
