#pragma once
class CUnit {
public:
	//index of the unit to which this unit¡¯s concentrate stream is connected 
	int conc_num;
	//index of the unit to which this unit¡¯s concentrate stream is connected 
	int tails_num;

	//A Boolean that is changed to true if the unit has been seen 
	bool mark;

	//represent the valuable matrial of feed
	double feed_val;
	//represent the waste matrial of feed
	double feed_waste;
	//represent the valuable matrial of feed
	double temp_val;
	//represent the waste matrial of feed
	double temp_waste;
	//valuable material in concentrate stream
	double conc_val;
	//waste material in concentrate stream
	double conc_waste;
	//valuable material in tailings stream
	double tail_val;
	//waste material in tailings stream
	double tail_waste;
	//store the old feed rate 
	double old_rate;


};

