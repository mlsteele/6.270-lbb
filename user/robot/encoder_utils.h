uint16_t encoder_distance_diff(uint16_t read_diff) {
	return 	(  155 // small wheel circumfrence in mm  
			 * (1./25.) // gear ratio 
			 * read_diff 
			 / 6) ;
}
uint16_t encoder_distance() {
	return encoder_distance_diff(encoder_read(ENCODER_PIN));
}
uint16_t encoder_speed() {
	//mm/s according to 100ms sample
	uint16_t first_read = encoder_read(ENCODER_PIN);
	pause(100);
	uint16_t second_read = encoder_read(ENCODER_PIN);
	return encoder_distance_diff(second_read - first_read) * 10;
}