




















// the function called by the button press
function get_output() {
	get_request('php_get_handler.php', draw_output, draw_error, '');
	return false;
}

// the function called by the button press
function send_input(char) {
	get_request('php_get_handler.php', draw_command_sent, draw_error, 'char='+char);
	return false;
}

// what to do if theres an error
// note that until it is succsessful, it assumes theres an error, so while its loading it assumes it just didnt work
// but it will update if it succsessfully recieves a reply
function draw_error() {
	document.getElementById("changeme").innerHTML = "ERROR!";
}

// what do do if its succsessful
function draw_output(response_text) {
	document.getElementById("raw_output").innerHTML = response_text;
}

// from when the buttons r clicked
function draw_command_sent(response_text){
	document.getElementById("control").innerHTML = response_text;
}

// get the request from the python CGI page
function get_request(url, success, error, data_string) {
	var req = new XMLHttpRequest();
	req.onreadystatechange = function () {
		return req.status == 200 ? success(req.responseText) : error(req.status);
	}
	req.open("POST", url, true);
	req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	req.send(data_string);
	return req;
}


















