public fun WE7275__add(in _path, out _nidout)
{
    DevAddStart(_path, 'WE7275', 211, _nidout);
    DevAddNode(_path // ':COMMENT', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':RACK', 'TEXT', *, *, _nid);
	DevAddNode(_path // ':SLOT', 'NUMERIC', 1, *, _nid);
	DevAddNode(_path // ':CLOCK_MODE', 'TEXT', 'BUSCLK', *, _nid);
	DevAddNode(_path // ':CLOCK_SOURCE', 'NUMERIC', *, *, _nid);
	DevAddNode(_path // ':FREQUENCY', 'NUMERIC', 100e3, *, _nid);
	DevAddNode(_path // ':TRIG_MODE', 'TEXT', 'BUSTRIG 1', *, _nid);
	DevAddNode(_path // ':TRIG_COM', 'TEXT', 'OR', *, _nid);
	DevAddNode(_path // ':PRE_TRIGGER', 'NUMERIC', *, *, _nid);
	DevAddNode(_path // ':HOLD_OFF', 'NUMERIC', 5000, *, _nid);
	DevAddNode(_path // ':ACQ_MODE', 'TEXT', 'TRIGGERED', *, _nid);
	DevAddNode(_path // ':ACQ_OVER', 'TEXT', 'OFF', *, _nid);
	DevAddNode(_path // ':REC_LENGTH', 'NUMERIC', *, *, _nid);
	DevAddNode(_path // ':MEM_PART', 'NUMERIC', 1, *, _nid);
	DevAddNode(_path // ':CHAN_ACTIVE', 'NUMERIC', 2, *, _nid);
    DevAddNode(_path // ':USE_TIME', 'TEXT', 'TRUE', *, _nid);

 	for (_c = 1; _c <=9; _c++)
	{
		_cn = _path // '.CHANNEL_0' // TEXT(_c, 1);
		DevAddNode(_cn, 'STRUCTURE', *, *, _nid);
    	DevAddNode(_cn // ':START_TIME', 'NUMERIC', 0., *, _nid);
		DevAddNode(_cn // ':END_TIME', 'NUMERIC', 1., *, _nid);
		DevAddNode(_cn // ':START_IDX', 'NUMERIC', *, *, _nid);
		DevAddNode(_cn // ':END_IDX', 'NUMERIC', *, *, _nid);
		DevAddNode(_cn // ':COUPLING', 'TEXT', 'DC', *, _nid);
		DevAddNode(_cn // ':RANGE', 'NUMERIC', 1.0, *, _nid);
		DevAddNode(_cn // ':TRIG_TYPE', 'TEXT', 'OFF', *, _nid);
		DevAddNode(_cn // ':TRIG_LEVEL', 'NUMERIC', 0.0, *, _nid);
		DevAddNode(_cn // ':AAF', 'NUMERIC', 0, *, _nid);
		DevAddNode(_cn // ':FILTER', 'NUMERIC', 0, *, _nid);
		DevAddNode(_cn // ':DATA', 'SIGNAL', *, '/compress_on_put/nomodel_write', _nid);
	}

 	for (_c = 10; _c <=16; _c++)
	{
		_cn = _path // '.CHANNEL_' // TEXT(_c, 2);
		DevAddNode(_cn, 'STRUCTURE', *, *, _nid);
    	DevAddNode(_cn // ':START_TIME', 'NUMERIC', 0., *, _nid);
		DevAddNode(_cn // ':END_TIME', 'NUMERIC', 1., *, _nid);
		DevAddNode(_cn // ':START_IDX', 'NUMERIC', *, *, _nid);
		DevAddNode(_cn // ':END_IDX', 'NUMERIC', *, *, _nid);
		DevAddNode(_cn // ':COUPLING', 'TEXT', 'DC', *, _nid);
		DevAddNode(_cn // ':RANGE', 'NUMERIC', 1.0, *, _nid);
		DevAddNode(_cn // ':TRIG_TYPE', 'TEXT', 'OFF', *, _nid);
		DevAddNode(_cn // ':TRIG_LEVEL', 'NUMERIC', 0.0, *, _nid);
		DevAddNode(_cn // ':AAF', 'NUMERIC', 0, *, _nid);
		DevAddNode(_cn // ':FILTER', 'NUMERIC', 0, *, _nid);
		DevAddNode(_cn // ':DATA', 'SIGNAL', *, '/compress_on_put/nomodel_write', _nid);
	}


    DevAddAction(_path//':INIT_ACTION', 'PULSE_PREPARATION', 'INIT', 55,'ISIS_SERVER', getnci(_path, 'fullpath'), _nid);
    DevAddAction(_path//':STORE_ACTION', 'STORE','STORE', 50,'ISIS_SERVER',getnci(_path, 'fullpath'), _nid);

    DevAddEnd();
}
