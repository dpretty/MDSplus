public fun LASER_RU__trigger(as_is _nid, optional _method)
{
    private _K_CONG_NODES = 67;

	private _N_COMMENT = 1;
    private _N_RS232_PORT = 2;
    private _N_TRIG_MODE = 3;
    private _N_NUM_SHOTS = 4;
    private _N_ENERGY = 5;
    private _N_VOLTAGE_OSC = 6;
    private _N_VOLTAGE_A_1 = 7;
    private _N_VOLTAGE_A_2 = 8;
    private _N_VOLTAGE_A_3 = 9;

    private _N_BALANCE_P_1 = 10;
    private _N_BALANCE_P_2 = 11;
    private _N_BALANCE_P_3 = 12;

    private _N_DELAY_AMP = 13;
    private _N_DELAY_PULS_1 = 14;
    private _N_DELAY_PULS_2 = 15;
    private _N_DELAY_PULS_3 = 16;
    private _N_DELAY_PULS_4 = 17;
    private _N_DELAY_FIRE = 18;

    private _N_CALIBRATION = 19;
    private _N_CAL_KOSC = 20;
    private _N_CAL_KAMP_1 = 21;
    private _N_CAL_KAMP_2 = 22;
    private _N_CAL_KAMP_3 = 23;
    private _N_CAL_KBAL_1 = 24;
    private _N_CAL_KBAL_2 = 25;
    private _N_CAL_KBAL_3 = 26;

    private _N_OSC_PAR = 27;
	private _N_AMP_PAR_1 = 28;
    private _N_AMP_PAR_2 = 29;
    private _N_AMP_PAR_3 = 30;
    private _N_PBAL_PAR_1 = 31;
    private _N_PBAL_PAR_2 = 32;
    private _N_PBAL_PAR_3 = 33;

    private _N_DTIME1_PAR_1 = 34;
    private _N_DTIME1_PAR_2 = 35;
    private _N_DTIME1_PAR_3 = 36;
    private _N_DTIME1_PAR_4 = 37;
    private _N_DTIME1_PAR_4 = 38;
    private _N_DF1M_PAR_1 = 39;
    private _N_DF1M_PAR_2 = 40;

    private _K_NUM_BITS = 5;
	private _K_NUM_DATA = 8;
    private _K_NODES_PER_BIT = 2;
    private _N_BIT_0= 42;
    private _N_BIT_PRE_SHOT = 1;
    private _N_BIT_POST_SHOT = 1;

	private _N_DATA = 57;

	private _RS232_XONXOFF = 0;
    private _RS232_CTSDSR  = 1;

    private _TRIG_INTERNAL  = 0;
    private _TRIG_EXTERNAL  = 2;
    private _TRIG_AUTOMATIC = 3;
    private _TRIG_MANUAL    = 4;


    _port = if_error(data(DevNodeRef(_nid, _N_RS232_PORT)),(DevLogErr(_nid, "Missing RS232 Port"); abort();));

    _hComm = RS232Open(_port, "baud=1200 parity=N data=8 stop=1", 0, _RS232_XONXOFF, 13);
	if( _hComm == 0 )
	{
    	DevLogErr(_nid, "Cannot open RS232 port : "//RS232GetError() );
		abort();
	}

 	/*  Decode trigger mode parameter */

	DevNodeCvt(_nid, _N_TRIG_MODE, ['INTERNAL',  'EXTERNAL',  'AUTOMATIC', 'MANUAL'], [0, 1, 2, 3], _trig_mode = 0);



	if (_trig_mode == _TRIG_INTERNAL) 
	{   
	   /* Set CR as command terminator */
		_cmnd =  "dterm 13";
		if( LASER_RUSendCommand(_hComm, _nid,  _cmnd) < 0 )
		{
			RS232Close(_hComm);
			abort();
		}

		_cmnd = "hip";
		_retvalue = LASER_RUSendCommand(_hComm, _nid,  _cmnd);
		if(_retvalue < 0)
		{
			RS232Close(_hComm);
			abort();
		}

		if(_retvalue == 49) {
		    
			_cmnd = "fire1";
			if( LASER_RUSendCommand(_hComm, _nid,  _cmnd) < 0 )
			{
				RS232Close(_hComm);
				abort();
			}

			_cmnd = "hip";
			_i = 0;
			_retvalue = 0;
			while (_i < 10  && _retvalue != 240) 
			{
				wait(1.0);
				_retvalue = LASER_RUSendCommand(_hComm, _nid,  _cmnd);
				if(_retvalue < 0)
				{
					RS232Close(_hComm);
					abort();
				}
				_i++;
			}
			if(_i == 10 && _retvalue != 240)
			{
				DevLogErr(_nid, "Ruby laser do not fire"); 
			}

	    } else DevLogErr(_nid, "Ruby laser do not ready"); ;

	} 
	else DevLogErr(_nid, "Trigger mode must be internal to perform trigger operation" );

	RS232Close(_hComm);

	return (1);

}















