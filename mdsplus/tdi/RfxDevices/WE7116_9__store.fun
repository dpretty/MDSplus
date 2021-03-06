public fun WE7116_9__store(as_is _nid, optional _method)
{


    private _K_CONG_NODES =     220;
    private _N_HEAD =			0;
    private _N_COMMENT =		1;
    private _N_RACK =			2;
    private _N_SLOT =			3;
	private _N_CLOCK_MODE =		4;
	private _N_CLOCK_SOURCE =	5;
	private _N_FREQUENCY =		6;
	private _N_TRIG_MODE =		7;
	private _N_TRIG_SOURCE =	8;
	private _N_TRIG_TYPE =		9;
	private _N_TRIG_LEVEL_U =	10;
	private _N_TRIG_LEVEL_L =	11;
	private _N_TRIG_HIST =		12;
	private _N_PRE_TRIGGER =	13;
	private _N_HOLD_OFF =		14;
	private _N_ACQ_MODE =		15;
	private _N_REC_LENGTH =		16;
	private _N_MEM_PART =		17;
	private _N_CHAN_ACTIVE =	18;
    private _N_USE_TIME =		19;

	private _K_NODES_PER_CHANNEL = 11;
	private _N_CHANNEL_1= 20;

    private _N_CHAN_START_TIME =	1;
	private _N_CHAN_END_TIME =		2;
	private _N_CHAN_START_IDX =		3;
	private _N_CHAN_END_IDX =		4;
	private _N_CHAN_COUPLING =		5;
	private _N_CHAN_RANGE =			6;
	private _N_CHAN_OFFSET =		7;
	private _N_CHAN_FILTER =		8;
	private _N_CHAN_PROBE =			9;
	private _N_CHAN_DATA =			10;


    private _N_CNTRL_IP = 2;
    private _N_STATION_IP = 3;
    private _N_RACK_TRIG_SOURCE1 = 5;
    private _N_RACK_TRIG_SOURCE2 = 7;
    private _N_RACK_CLOCK_SOURCE = 10;
    private _N_ARM_SOURCE = 11;

	private _K_NODES_PER_SLOT = 3;
    private _N_SLOT_1		  = 12;
    private _N_TYPE_MODULE	  = 1;
    private _N_LINK_MODULE    = 2;

	private _K_CHAN_MEM = 1024 * 1024 * 4;

	private _K_NUM_SLOT    = 9;


write(*, "WE7116_9__store");

	private _all_ch_off = 1;
	_num_chans = 18;

    for( _i = 0; _i < _num_chans; _i++)
    {
		_head_channel = _N_CHANNEL_1 + (_i *  _K_NODES_PER_CHANNEL);
        if( DevIsOn(DevNodeRef(_nid, _head_channel)) )
        { 
			_all_ch_off = 0;
		}
	}

	if( _all_ch_off )
	{
		write(*, "All chennels OFF");
		return(1);
	}

	_error = 0;

	_slot_num = if_error(data(DevNodeRef(_nid, _N_SLOT)), -1);

    if(_slot_num <= 0 || _slot_num > _K_NUM_SLOT)
    {
    	DevLogErr(_nid, "Invalid slot number");
 		abort();
    }

    _rack     = DevNodeRef(_nid, _N_RACK);
    _rack_nid = if_error( compile(getnci(getnci(_rack, 'record'), 'fullpath')), ( _error = 1) );
	if(_error == 1)
	{
		DevLogErr(_nid, 'Cannot resolve WE7000 rack');
		abort();
	}

	_slot_nid = _N_SLOT_1 + (_slot_num - 1)* _K_NODES_PER_SLOT;

	_rack_field_nid =  _slot_nid + _N_TYPE_MODULE;

	_mod_type = if_error(data(DevNodeRef(_rack_nid, _rack_field_nid)), "");
    if(_mod_type != "WE7116")
    {
    	DevLogErr(_nid, "Invalid rack configuration");
 		abort();
    }

	
	_rack_field_nid =  _slot_nid + _N_LINK_MODULE;

	_link_mod = if_error(data(DevNodeRef(_rack_nid, _rack_field_nid)), 0);
    if(_link_mod <= 0 || _link_mod > _K_NUM_SLOT)
    {
    	DevLogErr(_nid, "Invalid linked module number");
 		abort();
    }



    _controller_ip = if_error(data(DevNodeRef(_rack_nid, _N_CNTRL_IP)), "");
    if(_controller_ip == "")
    {
    	DevLogErr(_nid, "Invalid controller address");
 		abort();
    }


    _station_ip = if_error(data(DevNodeRef(_rack_nid, _N_STATION_IP)), "");
    if(_station_ip == "")
    {
    	DevLogErr(_nid, "Invalid station address");
 		abort();
    }

	_smp_int = float(0.0);
    DevNodeCvt(_nid, _N_CLOCK_MODE, ['INTERNAL', 'EXTERNAL', 'BUSCLOCK'], [0,1,2], _clock_mode = 0);
	if(_clock_mode == 0)
	{
        _freq = data(DevNodeRef(_nid, _N_FREQUENCY));
		_smp_int = float(1./ _freq);
        _clock_val = make_range(*, *, _smp_int);
    	DevPut(_nid, _N_CLOCK_SOURCE, _clock_val);
	}

	if(_clock_mode == 1)
	{
        _clk = DevNodeRef(_nid, _N_CLOCK_SOURCE);
		_clock_val = if_error( execute('`_clk'), (_error = 1) );
		if(_error == 1)
		{
			DevLogErr(_nid, "Cannot resolve external clock source"); 
			abort();
		}
	}

	if(_clock_mode == 2)
	{
        _clk = DevNodeRef(_rack_nid, _N_RACK_CLOCK_SOURCE);
		_clock_val = if_error( execute('`_clk'), (_error = 1) );
		if(_error == 1)
		{
			DevLogErr(_nid, "Cannot resolve bus clock source"); 
			abort();
		}
	   	DevPut(_nid, _N_CLOCK_SOURCE, _clock_val);
	}



    DevNodeCvt(_nid, _N_TRIG_MODE, ['EXTERNAL', 'BUSTRIG 1', 'BUSTRIG 2'], [0,1,2], _trig_mode = 0);

	if(_trig_mode == 0)
	{
		_trig=if_error(data(DevNodeRef(_nid, _N_TRIG_SOURCE)), (_error = 1) );
		if(_error == 1)
		{
			DevLogErr(_nid, "Cannot resolve external trigger"); 
			abort();
		}
	}

	if(_trig_mode == 1)
	{
		_trig=if_error(data(DevNodeRef(_rack_nid, _N_RACK_TRIG_SOURCE1)), (_error = 1) );
		if(_error == 1)
		{
			DevLogErr(_nid, "Cannot resolve bus trigger 1 trigger"); 
			abort();
		}
	}

	if(_trig_mode == 2)
	{
		_trig=if_error(data(DevNodeRef(_rack_nid, _N_RACK_TRIG_SOURCE2)), (_error = 1) );
		if(_error == 1)
		{
			DevLogErr(_nid, "Cannot resolve bus trigger 2 trigger"); 
			abort();
		}
	}


/*
Un array di trigger definisce il numero di acquisizioni da eseguire
Se scalare una sola acquisizione attualmente non gestito

	_num_acq  = if_error(size(_trig), 0); 
*/
	_num_acq = 1;

	_hold_off = if_error(data(DevNodeRef(_nid, _N_HOLD_OFF)), 0);


	_mem_part = if_error(data(DevNodeRef(_nid, _N_MEM_PART)), 0);


     DevNodeCvt(_nid, _N_ACQ_MODE, ['AUTO', 'NORMAL'], [0,1], _acq_mode = 1);

 	_chan_active = if_error(data(DevNodeRef(_nid, _N_CHAN_ACTIVE)), 2);


	_num_chans = _link_mod * _chan_active;

    _rec_length = if_error(data(DevNodeRef(_nid, _N_REC_LENGTH)), -1);
    if(_rec_length == -1)
    {
    	DevLogErr(_nid, "Invalid record length");
 		abort();
    }

    if(_rec_length > _K_CHAN_MEM)
    {
		_rec_length = _K_CHAN_MEM;
    }


	_pre_trigger = if_error(data(DevNodeRef(_nid, _N_PRE_TRIGGER)), 0);



	_vResolution = zero(_num_acq, FT_FLOAT(0.0));
	_vOffset     = zero(_num_acq, FT_FLOAT(0.0));

	_data = zero(_rec_length * _num_acq, 0W);
	_b_size = _rec_length * 2;

    for(_i = 0; _i < _num_chans; _i++)
    {
		_head_channel = _N_CHANNEL_1 + (_i *  _K_NODES_PER_CHANNEL);

        if( DevIsOn(DevNodeRef(_nid, _head_channel)) )
        { 
			_end_idx   = data(DevNodeRef(_nid, _head_channel +  _N_CHAN_END_IDX));	
			_start_idx = data(DevNodeRef(_nid, _head_channel +  _N_CHAN_START_IDX));
			
			_error = 0;	

			_error = we7000->WE7116ReadChData(_controller_ip, _station_ip, val(_slot_num), val(_link_mod), 
											  val(_i+1), val(_num_acq), ref(_data), val(_b_size),
											  ref(_vResolution), ref(_vOffset));

			_range = data(DevNodeRef(_nid, _head_channel +  _N_CHAN_RANGE));

			_offset = data(DevNodeRef(_nid, _head_channel +  _N_CHAN_OFFSET));

/*
			DevNodeCvt(_nid, _head_channel + _N_CHAN_PROBE, [1,10,100,1000], [0,1,2,3], _probe = 0);
			Attualmente viene sempre impostato probe a 1
*/
			_probe = 1;

			if(! _error )
			{
	/* Build signal */

				_dim = make_dim(make_window(_start_idx, _end_idx, _trig), _clock_val);

				_sig_nid =  DevHead(_nid) + _head_channel +  _N_CHAN_DATA;
/*
write(*, "Record length ", (_rec_length ) );
write(*, "Pre trigger ", (_pre_trigger ) );
write(*, "Indice di partenza ", (_pre_trigger + _start_idx) );
write(*, "Indice di fine ", (_pre_trigger + _end_idx) );
*/

write(*, "offset ", FT_FLOAT(_vOffset[0]));
write(*, "gain ", FT_FLOAT(_vResolution[0]));


				_dataSig = _data[_pre_trigger + _start_idx: _pre_trigger + _end_idx: *];

/*
				_status = DevPutSignal(_sig_nid,  _vOffset[0], _vResolution[0], word(_data), 0, _end_idx - _start_idx - 1, _dim);
*/
				_status = DevPutSignal(_sig_nid, _vOffset[0] / _vResolution[0], _vResolution[0], word(_dataSig), 0, _end_idx - _start_idx, _dim);


				if(! _status)
				{
					DevLogErr(_nid, 'Error writing data in pulse file');
				}

			}
			else
			{
				_msg = repeat(" ", 1024);
				we7000->WE7116GetErrorMsg(ref(_msg));
				DevLogErr(_nid, _msg);
			}
		}

    }

    if( _error )
    {
	    _msg = repeat(" ", 1024);
		we7000->WE7116GetErrorMsg(ref(_msg));
	    DevLogErr(_nid, _msg);
		abort();
    }

	return(1);

}
