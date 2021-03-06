public fun LASER_NDRT__dump(as_is _nid, optional _method)
{
    private _K_CONG_NODES = 21;

    private _N_HEAD		= 0;
    private _N_COMMENT		= 1;
    private _N_IP_ADDRESS	= 2;
    private _N_PORT		= 3;
    private _N_RT_ADDRESS	= 4;
    private _N_TRIG_MODE	= 5;
    private _N_TRIG_SOURCE	= 6;
    private _N_RT_TRIG_MODE	= 7;
    private _N_NUM_PULSES	= 8;
    private _N_DELAY_PULSE	= 9;
    private _N_MAX_AMP		= 10;
    private _N_MIN_AMP		= 11;
    private _N_MAX_PHASE	= 12;
    private _N_MIN_PHASE	= 13;
    private _N_OSC		= 14;
    private _N_AMP		= 15;
    private _N_SLAB		= 16;

    private _K_EXT_DT		= 0;
    private _K_EXT_10_DT	= 1;
    private _K_EXT_RT		= 2;

    private _K_N7_AMP		= 0;
    private _K_N7_AMP_PH	= 1;
    private _K_RATIO_DOM_SEC	= 2;
    private _K_RATIO_DOM_SEC_PH	= 3;


    private _ASCII_MODE = 0;

   _error = 0;

    _ip = if_error(data(DevNodeRef(_nid, _N_IP_ADDRESS)), _error = 1);
    if(_error)
    {
		DevLogErr(_nid, "Missing IP address"); 
		abort();
    }

    _port = if_error(data(DevNodeRef(_nid, _N_PORT)), _error = 1);
    if(_error)
    {
		DevLogErr(_nid, "Missing TCP Port");
		abort();
    }

 
    _sock = TCPOpenConnection(_ip, _port, _ASCII_MODE, 2000, _swap=0);
    if(_sock == 0)
    {
		DevLogErr(_nid, "Cannot connect to remote instruments"); 
		abort();
    }
 

	if((_err_msg = TCPSendCommand(_sock, "ND_DUMP") ) != "")
	{
		TCPCloseConnection(_sock);
		DevLogErr(_nid, _err_msg); 
		abort();
	}


	TCPCloseConnection(_sock);

	return (1);

	


}















