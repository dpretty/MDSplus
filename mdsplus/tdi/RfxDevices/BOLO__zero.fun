ppublic fun BOLO__zero(as_is _nid, optional _method)
{{

    private _K_CONG_NODES = BOLO = 637;
	private _N_HEAD = 0;
    private _N_COMMENT = 1;
    private _N_CAL_EXP = 2;
    private _N_CAL_SHOT = 3;
    private _N_IP_ADDR = 4;
    private _N_FG = 5;
    private _N_FREQUENCY = 6;
    private _N_TRIG_SOURCE = 7;
    private _N_DURATION = 8;
    private _N_HOR_HEAD = 9;
    private _N_HEAD_POS = 10;

    private _K_NODES_PER_CHANNEL = 13;
    private _N_CHANNEL_0 = 11;
	private _N_CHAN_CHANNEL_ID = 1;
	private _N_CHAN_CARRIER_FLAG = 2;
    private _N_CHAN_FILTER = 3;
    private _N_CHAN_GAIN = 4;
    private _N_CHAN_REF_PHASE = 5;
    private _N_CHAN_CALIB_GAIN = 6;
    private _N_CHAN_CALIB_FILTER = 7;
    private _N_CHAN_SENS = 8;
    private _N_CHAN_TAU = 9;
	private _N_CHAN_STATUS = 10;
    private _N_CHAN_DATA = 11;
    private _N_CHAN_POWER = 12;

	private _K_CHANNELS		= 48;
	private _K_MOULE_RACK   = 12;


    _ip_addr = if_error(data(DevNodeRef(_nid, _N_IP_ADDR)), "");
	if(_ip_addr == "")
	{
    	DevLogErr(_nid, "Invalid Crate IP specification");
 	    abort();
	}



	DevNodeCvt(_nid, _N_HOR_HEAD, ["USED", "NOT USED"],[1, 0], _hor_head = 0);

   	_chan_id        = [];

	for(_i = 0; _i < _K_CHANNELS; _i++)
	{

          _chan_nid = _N_CHANNEL_0 + _i * _K_NODES_PER_CHANNEL;

		   if( ( _is_on == DevIsOn(DevNodeRef(_nid, _chan_nid)) && _i < 37) || 
			   ( _is_on && _i >= 37 && _hor_head == 1) ) /* Disabilito i canali della testa orizzontale se non utilizzati */
		   {
				_enabled = 1;
		   }
		   else
		   {
				_enabled = 0;
		   }

		  _id = TomoChanId( _i+1, _enabled, BoloChMapping() );
		  _chan_id = [_chan_id, _id];

	}


	write(*, "Initialize rack ", _ip_addr);

	_cmd = 'MdsConnect("'//_ip_addr//'")';
	_status = execute(_cmd);

	_errors = zero(_K_CHANNELS, 0);

	if(_status != 0)
    {
       _expr = "BoloHWZero(0, $)";  

       _errors = MdsValue(_expr, _chan_id, 0);
	
       _expr = "BoloHwStartAcq(_K_MODULE_RACK, "//_reduction//")" ;  
   	   MdsValue(_expr);

	   MdsDisconnect();

	   write(*, "Fine Initialize rack ", _ip_add);
	 }
	 else
     {   	
	 
		DevLogErr(_nid, "Cannot connect to VME rack");
	    abort();

     }


	 for(_i = 0; _i < _K_CHANNELS; _i++)
	 {

       _chan_nid = _N_CHANNEL_0 + _i * _K_NODES_PER_CHANNEL;

	   if( TomoChanIsActive(_chan_id[_i]) )
	   {
			if( _errors[ _i ] != 0 )
				DevLogErr(_nid,  TomoErrorMessage( _errors[ _i ] ) );
	   }
	 }

     return(1);

}
