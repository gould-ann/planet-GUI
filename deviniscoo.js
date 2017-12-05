





// the function called by the button press
function get_output() {
	get_request('php_get_handler.php', draw_output, do_nothing);
	return false;
}

// the function called by the button press
function send_input(char) {
	get_request('php_send_handler.php?char='+char, draw_command_sent, draw_error);
	return false;
}

// what to do if theres an error
// note that until it is succsessful, it assumes theres an error, so while its loading it assumes it just didnt work
// but it will update if it succsessfully recieves a reply
function draw_error() {
	document.getElementById("raw_output").innerHTML = "ERROR!";
}


// why do we need this?
// so we can ignore errors
// why should we do that?
// we shouldnt, but just roll with it :)
function do_nothing(){

}

// what do do if its succsessful
function draw_output(response_text) {
	// response_text = response_text.substring(30);
	document.getElementById("raw_output").innerHTML = "<br>" + response_text;
	get_info(response_text);

	

}

// from when the buttons r clicked
function draw_command_sent(response_text){
	document.getElementById("control").innerHTML = response_text;
}

// get the request from the python CGI page
function get_request(url, success, error) {
	var req = new XMLHttpRequest();
	req.onreadystatechange = function () {
		return req.status == 200 ? success(req.responseText) : error(req.status);
	}
	req.open("POST", url, true);
	req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	req.send();
	return req;
}

setInterval(get_output, 1000);




// object displaying

var canvas = document.getElementById("canvas");
ctx = canvas.getContext("2d");
var size = 500;

function draw_circle(color, coords, r) {
	ctx.strokeStyle = color;
	ctx.lineWidth = 1;
	ctx.moveTo(coords[0]+r+(size/2),coords[1]+(size/2));
	ctx.beginPath();
	ctx.arc(coords[0]+(size/2),coords[1]+(size/2),r,0,2*Math.PI);
	ctx.closePath();
	ctx.fillStyle = color;
	ctx.fill();
	ctx.stroke();
}

function draw_pole(center, radius){
	draw_circle("red", center, radius);
}

function draw_bump(center){
	draw_circle("green", center, 5); //TODO get actual radius of small objects here
}

function draw_drop(center) {
	draw_circle("purple", center, 5);
}

function draw_white_line(center) {
	draw_circle("black", center, 5);
}

function draw_history(center) {
	draw_circle("pink", center, 1);
}

var objects = [];
var current_x = 0;
var current_y = 0;

var direction = Math.PI / 2;


function get_info(data){
	console.log("getting data for: " + data);
	if(data.substring(0,1) == "0"){
		// manual sweep data
		parsed_data = JSON.parse(data.substring(1));
		for (var i = 0; i < parsed_data.length; i++) {
			objects.push({"x": current_x + parsed_data[i]["x"], "y": current_y + parsed_data[i]["y"], radius: parsed_data[i]["radius"], "type": "pole"}); //TODO account for length of roomba
		}
	}
	if(data.substring(0,1) == "1"){
		// dynamic sensor
		parsed_data = JSON.parse(data.substring(1));

		for (var i = 0; i < parsed_data["types"].length; i++) {
			objects.push({"x": current_x, "y": current_y, "type": parsed_data["types"][i]}); //TODO account for length of roomba
		}
	}
	if(data.substring(0,1) == "2"){
		// move up
		parsed_data = JSON.parse(data.substring(1));

		move_position(parsed_data["cm"]);
	}

	if(data.substring(0,1) == "3"){
		// turn
		parsed_data = JSON.parse(data.substring(1));

		change_direction(parsed_data["degrees"]);
	}
}


function display(){
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	for (var i = 0; i < objects.length; i++) {
		if(objects[i]["type"] == "pole"){
			draw_pole([objects[i]["x"], objects[i]["y"]], objects[i]["radius"]);
		}
		if(objects[i]["type"] == "bump"){
			draw_bump([objects[i]["x"], objects[i]["y"]]);
		}
		if(objects[i]["type"] == "drop"){
			draw_drop([objects[i]["x"], objects[i]["y"]]);
		}
		if(objects[i]["type"] == "white_line"){
			draw_white_line([objects[i]["x"], objects[i]["y"]]);
		}
		if(objects[i]["type"] == "history"){
			draw_history([objects[i]["x"], objects[i]["y"]]);
		}
	}
}


function change_direction(radians) {

	direction += radians * Math.PI/180;
}

function move_position(cm){
	for (var i = 0; i < cm; i += cm / 20) {
		current_x += Math.acos(direction % (Math.PI / 2)) * cm / 20;
		current_y += Math.asin(direction % (Math.PI / 2) - Math.PI/4) * cm / 20;
		objects.push({"x": current_x, "y": current_y, "type": "history"});
	}


	
	// console.log(Math.asin(direction % (Math.PI / 2) - Math.PI/4));
}

setInterval(display, 1000);

// get_info('0[{"x": 50, "y": 90, "radius": 10}, {"x": 10, "y": 90, "radius": 15}]');

// get_info('1{"types": ["bump"]}');
// get_info('3{"degrees": 45}');
// get_info('2{"cm": 50}');
// get_info('1{"types": ["white_line"]}');

// console.log(objects);




