//Private Structs
typedef struct StateVars{
	float q;
	float r;
	float x;
	float p;
	float k;
} kalman_state;

//Private global
kalman_state kstate;


/**
 * Set the initial values for the kalman filter
 * @param q 
 * @param r 
 * @param x 
 * @param p 
 * @param k 
 */
void k_filter_init(float q, float r, float x, float p, float k){
    kstate.q = q;
    kstate.r = r;
    kstate.x = x;
    kstate.p = p;
    kstate.k = k;
}

/**
 * Filter one measurement and update the kalman state
 * @param  measurement measurement to be filtered
 * @return             filtered value
 */
float k_filter_value(float measurement){
	kstate.p = kstate.p + kstate.q;
	kstate.k = kstate.p  / (kstate.p + kstate.r);
	kstate.x = kstate.x + kstate.k * ( measurement - kstate.x);
	kstate.p = (1 - kstate.k) * kstate.p;
	return kstate.x;
}
