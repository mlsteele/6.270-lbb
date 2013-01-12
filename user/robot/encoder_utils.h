uint16_t encoder_distance(uint16_t read_diff = 0) {
	return 	(  155 // small wheel circumfrence in mm  
			 * (3./25.) // gear ratio 
			 * (read_diff or encoder_read(ENCODER_PIN)) 
			 / 6) ;
}
uint16_t encoder_speed() {
	//mm/s according to 100ms sample
	uint16_t first_read = encoder_read(ENCODER_PIN);
	pause(100);
	uint16_t second_read = encoder_read(ENCODER_PIN);
	return encoder_distance(second_read - first_read) * 10;
}