for (var i = 0; i < 8; i++)
{
	if (gamepad_is_connected_oxi(i)){
		// Triggers controls vibrate
		gamepad_set_vibration_oxi(i, gamepad_button_value_oxi(i, gp_shoulderlb) / 255, gamepad_button_value_oxi(i, gp_shoulderrb) / 255);
		
		// Up/Down dpad controls deadzone
		var deadzone = gamepad_get_axis_deadzone_oxi(i);
		if (gamepad_button_check_pressed(i, gp_padu)){
			deadzone += 0.1;
		}
		else if (gamepad_button_check_pressed(i, gp_padd)){
			deadzone -= 0.1;
		}
		gamepad_set_axis_deadzone_oxi(i, deadzone);
	}
}