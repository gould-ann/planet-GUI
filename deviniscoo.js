




// called to get outputs and draw them all
function draw_all() {
	get_request('get_commands.php', draw_output, do_nothing);
	return false;
}

var is_scanning = false;
// the function called by the button press
function send_input(char) {
	get_request('php_send_handler.php?char='+char, draw_command_sent, do_nothing);
	if(char == 's')
		is_scanning = true;
	else
		is_scanning = false;
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
	if(response_text.length > 10){
		var all_text = "";
		var parsed_data = JSON.parse(response_text);

		for (var i = 0; i < Math.min(10-parsed_data.length, 10); i++) {
			all_text += "<br>";
		}
		for (var i = Math.max(0, parsed_data.length-10); i < parsed_data.length; i++) {
			all_text += JSON.stringify(parsed_data[i]) + "<br>";
		}
		document.getElementById("raw_output").innerHTML = all_text;
		display(response_text);
	}
	

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

// setInterval(get_outputs, 1000);
setInterval(draw_all, 1000);
// draw_all();
// setTimeout(draw_all, 1000);


// object displaying

var canvas = document.getElementById("canvas");
ctx = canvas.getContext("2d");
var size = 500;

function draw_circle(color, coords, r) {
	ctx.lineWidth = 1;
	ctx.fillStyle = color;
	ctx.moveTo(coords[0]+r,coords[1]);
	ctx.beginPath();
	ctx.arc(coords[0],coords[1],r,0,2*Math.PI);
	ctx.closePath();
	ctx.fillStyle = color;
	ctx.fill();
	ctx.stroke();
	ctx.fillStyle = "black";
}


function display(data){
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	var current_x = 500;
	var current_y = 500;
	var direction = 0;

	parsed_data = JSON.parse(data);

	for (var i = 0; i < parsed_data.length; i++) {
		if(parsed_data[i]["class"] == 0){
			// movement
			if(parsed_data[i]["type"] == "turn-right"){
				direction += Math.PI / 8;
			}
			if(parsed_data[i]["type"] == "turn-left"){
				direction -= Math.PI / 8;
			}
			if(parsed_data[i]["type"] == "move_forward"){
				var new_x = current_x + Math.cos(direction) * parsed_data[i]["distance"];
				var new_y = current_y + Math.sin(direction) * parsed_data[i]["distance"];
				ctx.beginPath();
				ctx.moveTo(current_x,current_y);
				ctx.lineTo(new_x,new_y);
				ctx.stroke();
		        current_x = new_x;
		        current_y = new_y;
			}
			if(parsed_data[i]["type"] == "move-down"){
				var new_x = current_x - Math.cos(direction) * parsed_data[i]["distance"];
				var new_y = current_y - Math.sin(direction) * parsed_data[i]["distance"];
				ctx.beginPath();
				ctx.moveTo(current_x,current_y);
				ctx.lineTo(new_x,new_y);
				ctx.stroke();
		        current_x = new_x;
		        current_y = new_y;
			}
		} else if(parsed_data[i]["class"] == 1) {
			var x = Math.cos(direction) * parsed_data[i]["distance"];
			var y = Math.sin(direction) * parsed_data[i]["distance"];

			if(parsed_data[i]["type"] == "white-line"){
				draw_circle("black", [current_x + x, current_y + y], 5);
			}
			if(parsed_data[i]["type"] == "cliff-right"){
				draw_circle("red", [current_x + x, current_y + y], 5);
			}
			if(parsed_data[i]["type"] == "cliff-left"){
				draw_circle("red", [current_x + x, current_y + y], 5);
			}
			if(parsed_data[i]["type"] == "bump-right"){
				draw_circle("blue", [current_x + x, current_y + y], 5);
			}
			if(parsed_data[i]["type"] == "bump-left"){
				draw_circle("blue", [current_x + x, current_y + y], 5);
			}
		} else if(parsed_data[i]["class"] == 2 && parsed_data[i]["width"] < 10) {
			var avg_angle = (180-((parsed_data[i]["start_angle"] + parsed_data[i]["end_angle"]) / 2)) / 180 * Math.PI;
			var x = current_x + Math.cos(avg_angle + direction - Math.PI/2) * parsed_data[i]["distance"];
			var y = current_y + Math.sin(avg_angle + direction - Math.PI/2) * parsed_data[i]["distance"];
			draw_circle("rgba(0,255,0,0.3)", [x,y], parsed_data[i]["width"]);
		}
	}

	// draw direction
	////////////////////////////////
	// find points A,B,C
	var temp_x = current_x - Math.cos(direction) * 10;
	var temp_y = current_y - Math.sin(direction) * 10;
	var aa = [temp_x + Math.cos(direction - Math.PI/2)*10, temp_y + Math.sin(direction - Math.PI/2)*10];
	var bb = [temp_x + Math.cos(direction)*10, temp_y + Math.sin(direction)*10];
	var cc = [temp_x + Math.cos(direction + Math.PI/2)*10, temp_y + Math.sin(direction + Math.PI/2)*10];

	// draw lines
	ctx.strokeStyle = "red";
	ctx.lineWidth = 3;
	ctx.beginPath();
	ctx.moveTo(aa[0],aa[1]);
	ctx.lineTo(bb[0],bb[1]);
	ctx.stroke();

	ctx.beginPath();
	ctx.moveTo(bb[0],bb[1]);
	ctx.lineTo(cc[0],cc[1]);
	ctx.stroke();
	ctx.lineWidth = 1;
	ctx.strokeStyle = "black";
	//////////////////////////////
	
	// draw scan cone
	//////////////////////////////
	console.log(is_scanning);
	if(is_scanning) {
		var cone_a = [current_x - Math.cos(direction - 3*Math.PI/8)*40, current_y - Math.sin(direction - 3*Math.PI/8)*40];
		var cone_b = [current_x - Math.cos(direction + 3*Math.PI/8)*40, current_y - Math.sin(direction + 3*Math.PI/8)*40];
		ctx.strokeStyle = "rgba(0,0,255,0.3)";
		ctx.moveTo(current_x, current_y);
		ctx.beginPath();
		ctx.arc(current_x,current_y,40,direction - 3*Math.PI/8,direction + 3*Math.PI/8);
		ctx.lineTo(current_x, current_y);
		ctx.closePath();
		ctx.fillStyle = "rgba(0,0,255,0.3)";
		ctx.fill();
		ctx.stroke();
		ctx.strokeStyle = "black";
	}
	//////////////////////////////
	
}


// get_info('0[{"x": 50, "y": 90, "radius": 10}, {"x": 10, "y": 90, "radius": 15}]');

// get_info('1{"types": ["bump"]}');
// get_info('3{"degrees": 45}');
// get_info('2{"cm": 50}');
// get_info('1{"types": ["white_line"]}');

// console.log(objects);




