<?php

	$output = shell_exec("echo [;sed '$ s/<br>//g;  s/<br>/,/g;' roomba_history.txt;echo ]");
	echo $output;
?> 