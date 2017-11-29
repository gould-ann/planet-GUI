<?php
	$output = shell_exec('/usr/bin/python send_data.py '.$_GET["char"]);
	echo $output;
?> 
