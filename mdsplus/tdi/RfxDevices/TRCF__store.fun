public fun TRCF__store(as_is _nid, optional _method)
{
    private _K_CONG_NODES = 45;
    private _N_HEAD = 0;
    private _N_NAME = 1;
    private _N_COMMENT = 2;
    private _N_CHANNELS = 3;
    private _N_CHAN_OFFSET = 4;
    private _N_CLOCK_MODE = 5;
    private _N_TRIG_SOURCE = 6;
    private _N_CLOCK_SOURCE = 7;
    private _N_FREQUENCY = 8;
    private _N_USE_TIME = 9;
    private _N_PTS = 10;

    private _K_NODES_PER_CHANNEL = 6;
    private _N_CHANNEL_0= 11;
    private _N_CHAN_START_TIME = 1;
    private _N_CHAN_END_TIME = 2;
    private _N_CHAN_START_IDX = 3;
    private _N_CHAN_END_IDX = 4;
    private _N_CHAN_DATA = 5;

    private _N_INIT_ACTION = 47;
    private _N_STORE_ACTION = 48;
    private _256K = 262144;
    private _128K = 131072;
    private _64K = 65536;
    private _32K = 32768;
    private _16K = 16384;

    _name = DevNodeRef(_nid, _N_NAME);
 
 
    DevCamChk(_name, CamPiow(_name, 2,0, _control=0, 16),1,1);
    _not_stopped = ((_control >> 13) &3);
    if (_not_stopped != 0)
    {
        DevLogErr(_nid, 'Module is not in STOP state');
        return (0);
    }

 /*Check if CADF or TRCF */
    _status=DevCamChk(_name, CamPiow(_name, 2,0, _control_reg,24),1,*); 
	if((_control_reg & (1 << 16)) == 0)
	{
		write(*, 'CADF');
		_buffer_size = _64K;
	}
	else
	{
		write(*, 'TRCF');
		_buffer_size = _256K;
	}

    _num_chans = data(DevNodeRef(_nid, _N_CHANNELS));

    _trig = data(DevNodeRef(_nid, _N_TRIG_SOURCE));
    _clock = evaluate(DevNodeRef(_nid, _N_CLOCK_SOURCE));
    _clock = execute('evaluate(`_clock)');
    _pts = data(DevNodeRef(_nid, _N_PTS));

write(*, 'PTS', _pts);

    DevCamChk(_name, CamPiow(_name, 0,0,_base_mar=0, 24),1,1);

write(*, 'MAR', _base_mar);

    for(_i = 0; _i < _num_chans; _i++)
    {
        if(DevIsOn(DevNodeRef(_nid, _N_CHANNEL_0 +(_i *  _K_NODES_PER_CHANNEL))))
        { 
	    _end_idx = data(DevNodeRef(_nid, _N_CHANNEL_0  +(_i *  _K_NODES_PER_CHANNEL) +  _N_CHAN_END_IDX));	
	    _start_idx = data(DevNodeRef(_nid, _N_CHANNEL_0  +(_i *  _K_NODES_PER_CHANNEL) +  _N_CHAN_START_IDX));	

write(*,'Start idx', _start_idx, 'End idx', _end_idx);

/*Check and correct memory overflow or wron setting*/
	    if(_end_idx > _pts) _end_idx = _pts;
		switch(_num_chans)
		{
			case (16)
				_max_samples = _buffer_size / 16;
				break;
			case (8)
				_max_samples = _buffer_size / 8;
				break;
			case (4)
				_max_samples = _buffer_size / 4;
				break;
			case (2)
				_max_samples = _buffer_size / 2;
				break;
			case (1)
				_max_samples = _buffer_size;
				break;
		}
	    if(_end_idx  - _start_idx >  _max_samples) _start_idx = _pts - _max_samples;
	    if(_end_idx < _start_idx) _start_idx = _end_idx - 1;

/* Compute MAR */
/*	    if(_num_chans == 6)
	        _mar = _base_mar - 2*(_pts - _start_idx);
	    else
	        _mar = _base_mar - (_pts - _start_idx);
	    if(_mar < 0) _mar = _mar + _1M;
	    if(_num_chans == 3)
	        _mar = _mar |(_i   << 20);
	    else
	        _mar = (_mar + mod(_i, 2))|((_i /2)<<20);

*/
write(*, 'NUM CHANS: ', _num_chans);

	    _mar = _base_mar - _num_chans * (_pts - _start_idx);
	    if(_mar < 0) _mar = _mar + _256K;
    	    DevCamChk(_name, CamPiow(_name, 0,16,_mar, 24),1,1);
	    DevCamChk(_name, CamFstopw(_name, 0, 2, _end_idx - _start_idx, _data=0, 16), 1, *);

	    _dim = make_dim(make_window(_start_idx, _end_idx, _trig), _clock);
	    _sig_nid =  DevHead(_nid) + _N_CHANNEL_0  +(_i *  _K_NODES_PER_CHANNEL) +  _N_CHAN_DATA;

	    _status = DevPutSignal(_sig_nid, 0, 5/2048., word(_data), 0, _end_idx - _start_idx - 1, _dim);
	    if(! _status)
	    {
                DevLogErr(_nid, 'Error writing data in pulse file');
                return(0);
            }
	}
    }
    return(1);
}
