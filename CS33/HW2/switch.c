// Problem 3.63 reverse engineer a switch statement from machine code
//
long switch_prob(long x, long n){
    long result = x;
    switch(n) {
		case 60: 
            result = x * 8;
		    break;
		case 62: 
            result = x * 8;
			break;
		case 63: 
            result = x;
			result = result >> 0x3;
			break;
		case 64: 
            result = x;
			result = result << 0x4;
			result = result - x;
			x = result;
			x *= x;
			result = x + 75;
			break;
		case 65: 
            x *= x;
			result = x + 75;
			break;
		default: 
            result = x + 75;
    }
    return result;
}
