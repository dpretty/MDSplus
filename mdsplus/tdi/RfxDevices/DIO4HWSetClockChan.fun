public fun DIO4HWSetClockChan(in _nid, in _board_id, in _channel, in _frequency, in _duty_cycle)
{

	private _DIO4_CLOCK_SOURCE_INTERNAL	=	0x0;
	private _DIO4_CLOCK_SOURCE_TIMING_HIGHWAY =	0x3;
	private _DIO4_CLOCK_SOURCE_RISING_EDGE	=	0x0;
	private _DIO4_ER_INT_DISABLE	= 0x0;
	private _DIO4_TC_TRIGGER_DISABLED = 0x00;
	private _DIO4_TC_GATE_DISABLED	=			0x00;
	private _DIO4_TC_INT_DISABLE	=		0x00;
	private _DIO4_TC_GATE_DISABLED = 0x00;
	private _DIO4_TC_SINGLE_SHOT = 0;	


	_period = 1./_frequency;
	_tot_cycles = long(_period / 1E-7 + 0.5);

	_cycles_1 = long(_tot_cycles * _duty_cycle / 100.) - 1; 
	if(_cycles_1 < 0) _cycles_1 = 0;

	_cycles_2 = long(_tot_cycles - _cycles_1 - 2); 
	if(_cycles_2 < 0) _cycles_2 = 0;


	_cycles = [_cycles_1, _cycles_2, 0, 0];

	_levels = [byte(1),byte(0),byte(1),byte(0)];



/* Initialize Library if the first time */
    if_error(_DIO4_initialized, (DIO4->DIO4_InitLibrary(); public _DIO4_initialized = 1;));


/* Open device */
	_handle = 0L;
	_status = DIO4->DIO4_Open(val(long(_board_id)), ref(_handle));
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error opening DIO4 device, board ID = "// _board_id);
		else
			write(*, "Error opening DIO4 device, board ID = "// _board_id);
		return(0);
	}



/* No trigger */
	_status = DIO4->DIO4_TC_SetTrigger(val(_handle), val(byte(_channel + 1)), val(byte(_DIO4_TC_TRIGGER_DISABLED)), val(byte(0)),
		val(byte(0)));
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error setting trigger in DIO4 device, board ID = "// _board_id);
		else
			write(*, "Error setting trigger in DIO4 device, board ID = "// _board_id);
		return(0);
	}

/* No Gating */
	_status = DIO4->DIO4_TC_SetGate(val(_handle), val(byte(_channel + 1)), val(byte(_DIO4_TC_GATE_DISABLED)), val(byte(0)),
		val(byte(0)), val(byte(0)));
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error setting trigger in DIO4 device, board ID = "// _board_id);
		else
			write(*, "Error setting trigger in DIO4 device, board ID = "// _board_id);
		return(0);
	}

/* Phase setting */
	_status = DIO4->DIO4_TC_SetPhaseSettings(val(_handle), val(byte(_channel + 1)), val(byte(_DIO4_TC_SINGLE_SHOT)), 
		val(byte(_DIO4_TC_INT_DISABLE)), _levels);
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error setting phase in DIO4 device, board ID = "// _board_id);
		else
			write(*, "Error setting phase in DIO4 device, board ID = "// _board_id);
		return(0);
	}


/* Timing setting */
	_status = DIO4->DIO4_TC_SetPhaseTiming(val(_handle), val(byte(_channel + 1)), _cycles, val(long(0)), 
		val(long(0)));
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error setting phase timing in DIO4 device, board ID = "// _board_id);
		else
			write(*, "Error setting phase timing in DIO4 device, board ID = "// _board_id);
		return(0);
	}
	

/* Close device */
	DIO4->DIO4_Close(val(_handle));

    return(1);
}

