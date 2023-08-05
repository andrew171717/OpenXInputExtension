draw_set_color(c_white);
draw_text(1,1, "OpenXInputExtension Example Project v1");
draw_text(1,20,"Devices Connected: " + string(gamepad_get_device_count_oxi()));
draw_text(1,40,"Controls: Pressing triggers will vibrate controllers. Dpad Up/Down will change axis deadzone.");
draw_text(1,60,"Legend: C = gamepad_button_check_oxi; P = gamepad_button_check_pressed_oxi; R = gamepad_button_check_released_oxi; V = gamepad_button_value_oxi;");

var _y_start = 100;
var _y_off = 20;
var _x_off = 172;
var _y = _y_start;
var _x = 1;
for (var i = 0; i < 8; i++)
{
	draw_text(_x, _y, "Device [" + string(i) + "]"); _y += _y_off;
	var connected = gamepad_is_connected_oxi(i);
	draw_set_color(connected ? c_lime : c_red);
	draw_text(_x, _y, connected ? "Connected" : "Not Connected"); _y += _y_off;
	draw_set_color(c_white);
	draw_text(_x, _y, "Deadzone : " + string(gamepad_get_axis_deadzone_oxi(i))); _y += _y_off; _y += _y_off;
	draw_text(_x, _y, "Button   : CPR[V]"); _y += _y_off; 
	draw_text(_x, _y, "A        : " + string(gamepad_button_check_oxi(i, gp_face1)) + string(gamepad_button_check_pressed_oxi(i, gp_face1)) + string(gamepad_button_check_released_oxi(i, gp_face1))); _y += _y_off;
	draw_text(_x, _y, "B        : " + string(gamepad_button_check_oxi(i, gp_face2)) + string(gamepad_button_check_pressed_oxi(i, gp_face2)) + string(gamepad_button_check_released_oxi(i, gp_face2))); _y += _y_off;
	draw_text(_x, _y, "X        : " + string(gamepad_button_check_oxi(i, gp_face3)) + string(gamepad_button_check_pressed_oxi(i, gp_face3)) + string(gamepad_button_check_released_oxi(i, gp_face3))); _y += _y_off;
	draw_text(_x, _y, "Y        : " + string(gamepad_button_check_oxi(i, gp_face4)) + string(gamepad_button_check_pressed_oxi(i, gp_face4)) + string(gamepad_button_check_released_oxi(i, gp_face4))); _y += _y_off;
	draw_text(_x, _y, "LShoulder: " + string(gamepad_button_check_oxi(i, gp_shoulderl)) + string(gamepad_button_check_pressed_oxi(i, gp_shoulderl)) + string(gamepad_button_check_released_oxi(i, gp_shoulderl))); _y += _y_off;
	draw_text(_x, _y, "RShoulder: " + string(gamepad_button_check_oxi(i, gp_shoulderr)) + string(gamepad_button_check_pressed_oxi(i, gp_shoulderr)) + string(gamepad_button_check_released_oxi(i, gp_shoulderr))); _y += _y_off;
	draw_text(_x, _y, "LTrigger : " + string(gamepad_button_check_oxi(i, gp_shoulderlb)) + string(gamepad_button_check_pressed_oxi(i, gp_shoulderlb)) + string(gamepad_button_check_released_oxi(i, gp_shoulderlb)) + "["+string(gamepad_button_value_oxi(i, gp_shoulderlb)) + "]"); _y += _y_off;
	draw_text(_x, _y, "RTrigger : " + string(gamepad_button_check_oxi(i, gp_shoulderrb)) + string(gamepad_button_check_pressed_oxi(i, gp_shoulderrb)) + string(gamepad_button_check_released_oxi(i, gp_shoulderrb)) + "["+string(gamepad_button_value_oxi(i, gp_shoulderrb)) + "]"); _y += _y_off;
	draw_text(_x, _y, "Up       : " + string(gamepad_button_check_oxi(i, gp_padu)) + string(gamepad_button_check_pressed_oxi(i, gp_padu)) + string(gamepad_button_check_released_oxi(i, gp_padu))); _y += _y_off;
	draw_text(_x, _y, "Down     : " + string(gamepad_button_check_oxi(i, gp_padd)) + string(gamepad_button_check_pressed_oxi(i, gp_padd)) + string(gamepad_button_check_released_oxi(i, gp_padd))); _y += _y_off;
	draw_text(_x, _y, "Left     : " + string(gamepad_button_check_oxi(i, gp_padl)) + string(gamepad_button_check_pressed_oxi(i, gp_padl)) + string(gamepad_button_check_released_oxi(i, gp_padl))); _y += _y_off;
	draw_text(_x, _y, "Right    : " + string(gamepad_button_check_oxi(i, gp_padr)) + string(gamepad_button_check_pressed_oxi(i, gp_padr)) + string(gamepad_button_check_released_oxi(i, gp_padr))); _y += _y_off;
	draw_text(_x, _y, "Start    : " + string(gamepad_button_check_oxi(i, gp_start)) + string(gamepad_button_check_pressed_oxi(i, gp_start)) + string(gamepad_button_check_released_oxi(i, gp_start))); _y += _y_off;
	draw_text(_x, _y, "Back     : " + string(gamepad_button_check_oxi(i, gp_select)) + string(gamepad_button_check_pressed_oxi(i, gp_select)) + string(gamepad_button_check_released_oxi(i, gp_select))); _y += _y_off;
	draw_text(_x, _y, "LThumb   : " + string(gamepad_button_check_oxi(i, gp_stickl)) + string(gamepad_button_check_pressed_oxi(i, gp_stickl)) + string(gamepad_button_check_released_oxi(i, gp_stickl))); _y += _y_off;
	draw_text(_x, _y, "RThumb   : " + string(gamepad_button_check_oxi(i, gp_stickr)) + string(gamepad_button_check_pressed_oxi(i, gp_stickr)) + string(gamepad_button_check_released_oxi(i, gp_stickr))); _y += _y_off;
	_y += _y_off; draw_text(_x, _y, "Axis     : Value"); _y += _y_off; 
	draw_text(_x, _y, "Left H   : " + string(gamepad_axis_value_oxi(i, gp_axislh))); _y += _y_off;
	draw_text(_x, _y, "Left V   : " + string(gamepad_axis_value_oxi(i, gp_axislv))); _y += _y_off;
	draw_text(_x, _y, "Right H  : " + string(gamepad_axis_value_oxi(i, gp_axisrh))); _y += _y_off;
	draw_text(_x, _y, "Right V  : " + string(gamepad_axis_value_oxi(i, gp_axisrv))); _y += _y_off;
	_y = _y_start;
	_x += _x_off;
}