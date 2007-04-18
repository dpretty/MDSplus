public fun RfxRotPertConfig(in _system, in _type, in _idx)
{
	
	
	private fun rotRelative(in _val)
	{
		_out = "ERR";
		
		switch(_val)
		{
			case(0)
				_out = "ABS.";
			break;
			
			case(1)
				_out = "Rel. Bt";
			break;

			case(2)
				_out = "Rel. Br";
			break;
		}
		return ( _out ) ;
	};
	
	private fun phaseModuleSignal(in _type, in _ts, in _te, in _tr, in _amp, in _freq, in _fase)
	{
		
		_segno = 1.0;
		
		_fase = mod( _fase, 2 * $pi);
		
		if( _freq < 0 )
		{
		   _freq = -_freq;
		   _segno = 0.0;
		}
		
		write(*, _ts, _te, _tr, _amp, _freq, _fase);
		
		if(  _ts + _tr > _te ) _tr = _te - _ts;
		

		if( _type == "MODULE" )
		{						
			_out = if_error( make_signal( [0, 0, _amp, _amp, 0, 0] , , [ fs_float(\RFX::T_START_PR), _ts, _ts + _tr, _te, _te, fs_float(\RFX::T_STOP_PR) ] ), _zeroSig );
			
		}
		else
		{
			_x = [_ts];
			_y = [_fase];
			
			if(_freq != 0.0)
			{
				_period = 1./_freq;
			}
			else
			{
				_out = if_error( make_signal( [0, 0, _fase, _fase, 0, 0] , , [fs_float(\RFX::T_START_PR), _ts, _ts, _te, _te, fs_float(\RFX::T_STOP_PR)] ), _zeroSig );
				return (_out);
			}
			
			_t = _ts + _period;
	
			while(_t < _te)
			{
				_x = [ _x, _t , _t  ];
				_y = [ _y,  _segno * 2 * $pi, (1 - _segno) * 2 * $pi];
				_t += _period;
			}
			_t -= _period;
			_x = [ _x, _te];
			
			if( _segno )
				_y = [ _y, 2 * $pi * ((_te - _t)/( _period )), 0];
			else
				_y = [ _y, - 2 * $pi * ((_te - _t)/( _period )) + 2*$pi, 0];
			
			_out = if_error( make_signal( _y , , _x ), _zeroSig );						
		}
		
		return ( _out );
  	};
	
	
	_path = "";
	
	switch( _system )
	{
		case ("EDA3")
			_path = "\\EDA3::CONTROL";
		break;
		case ("MHD_AC")
			_path = "\\MHD_AC::CONTROL";
		break;
		case ("MHD_BC")
			_path = "\\MHD_BC::CONTROL";
		break;
	}


	_mode = execute(_path//":TRIG1_CONTR");

	if( $shot < 19359 && $shot > 1000 && _idx > 4 )
	{
		write(*, _idx);
		_idx = _idx + 1000;
	}

    if ( _type == "TITLE" )
	{
	    _modeName = RfxControlNameToIdx( _mode );
		if( _modeName ==  "ROT.PERTURBATION" || _modeName ==  "MODE CONTROL+ROT.PERT." || _modeName ==  "VS+ROT.PERT.(FEEDBACK)" || _modeName == "VS+ROT.PERT.(FEEDFORW)" || _modeName ==  "Closer VS + Rot.Pert.")
		{
			switch( _idx )
			{
				case (1)
					if($shot < 19359 && $shot > 1000)
						return ( "R P 1  N = "//trim(adjustl(execute(_path//".PARAMETERS:PAR97_VAL")))//" M = "//trim(adjustl(execute(_path//".PARAMETERS:PAR98_VAL")))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR102_VAL"))) );
					else
						return ( "R P 1  N = "//trim(adjustl(execute(_path//".PARAMETERS:PAR97_VAL")))//" M = "//trim(adjustl(execute(_path//".PARAMETERS:PAR98_VAL")))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR102_VAL")))//" "//rotRelative(execute(_path//".PARAMETERS:PAR251_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR255_VAL"))) );
				break;			
				case (2)
					if($shot < 19359 && $shot > 1000)
						return ( "R P 2  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR104_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR105_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR109_VAL"))) );
					else
						return ( "R P 2  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR104_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR105_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR109_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR252_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR256_VAL"))) );
				break;
				case (3)
					if($shot < 19359 && $shot > 1000)
						return ( "R P 3  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR111_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR112_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR116_VAL")))  );
					else
						return ( "R P 3  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR111_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR112_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR116_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR253_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR257_VAL"))) );
				break;
				case (4)
					if($shot < 19359 && $shot > 1000)				
						return ( "R P 4  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR118_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR119_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR123_VAL")))  );
					else
						return ( "R P 4  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR118_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR119_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR123_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR254_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR258_VAL"))) );
				break;
				case (5)
					return ( "R P 5  N = "//trim(adjustl(execute(_path//".PARAMETERS:PAR259_VAL")))//" M = "//trim(adjustl(execute(_path//".PARAMETERS:PAR260_VAL")))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR264_VAL")))//" "//rotRelative(execute(_path//".PARAMETERS:PAR267_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR268_VAL"))) );
				break;			
				case (6)
					return ( "R P 6  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR269_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR270_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR274_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR277_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR278_VAL"))) );
				break;
				case (7)
					return ( "R P 7  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR279_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR280_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR284_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR287_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR288_VAL"))) );
				break;
				case (8)
					return ( "R P 8  N = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR289_VAL"))))//" M = "//trim(adjustl(int(execute(_path//".PARAMETERS:PAR290_VAL"))))//"  f = "//trim(adjustl(execute(_path//".PARAMETERS:PAR294_VAL"))) //" "//rotRelative(execute(_path//".PARAMETERS:PAR297_VAL"))//" tr. = "//trim(adjustl(execute(_path//".PARAMETERS:PAR298_VAL"))) );
				break;
				
				case default 
					return("Segnale definito dallo shot 19359");
			}
		}
		else
		{
			return ("Rotating Perturbation OFF" );
		}
	}
	
	
    if ( _type == "MODULE" || _type == "PHASE" )
	{
		_zeroSig = make_signal([0.,0],,[-1., 1.]);


	    _modeName = RfxControlNameToIdx( _mode );
		if( _modeName ==  "ROT.PERTURBATION" || _modeName ==  "MODE CONTROL+ROT.PERT." || _modeName ==  "VS+ROT.PERT.(FEEDBACK)" || _modeName == "VS+ROT.PERT.(FEEDFORW)" || _modeName ==  "Closer VS + Rot.Pert.")
		{
			switch( _idx )
			{
				case (1)
					_ts = if_error( execute(_path//".PARAMETERS:PAR100_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR101_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR232_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR99_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR102_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR103_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                    if(size(data( _out )) == 1) _out =  _zeroSig;  
				    return( _out );
				break;
				case (2)
					_ts = if_error( execute(_path//".PARAMETERS:PAR107_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR108_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR233_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR106_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR109_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR110_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);
								
                    if(size(data( _out )) == 1) _out =  _zeroSig;  
				    return( _out );
				break;
				case (3)
					_ts = if_error( execute(_path//".PARAMETERS:PAR114_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR115_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR234_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR113_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR116_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR117_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                    if(size(data( _out )) == 1) _out =  _zeroSig;  
  				    return( _out );
				break;
				case (4)

					_ts = if_error( execute(_path//".PARAMETERS:PAR121_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR122_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR235_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR120_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR123_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR124_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                   if(size(data( _out )) == 1) _out =  _zeroSig;  
				   return( _out );
				break;
				case (5)

					_ts = if_error( execute(_path//".PARAMETERS:PAR262_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR263_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR266_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR261_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR264_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR265_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                   if(size(data( _out )) == 1) _out =  _zeroSig;  
				   return( _out );
				break;
				case (6)

					_ts = if_error( execute(_path//".PARAMETERS:PAR272_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR273_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR276_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR271_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR274_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR275_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                   if(size(data( _out )) == 1) _out =  _zeroSig;  
				   return( _out );
				break;
				case (7)

					_ts = if_error( execute(_path//".PARAMETERS:PAR282_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR283_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR286_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR281_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR284_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR285_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                   if(size(data( _out )) == 1) _out =  _zeroSig;  
				   return( _out );
				break;
				case (8)

					_ts = if_error( execute(_path//".PARAMETERS:PAR292_VAL"), 0);
					_te = if_error( execute(_path//".PARAMETERS:PAR293_VAL"), 0);
					_tr = if_error( execute(_path//".PARAMETERS:PAR296_VAL"), 0);
					_amp = if_error( execute(_path//".PARAMETERS:PAR291_VAL"), 0);
					_freq = if_error( execute(_path//".PARAMETERS:PAR294_VAL"), 0);
					_fase = if_error( execute(_path//".PARAMETERS:PAR295_VAL"), 0);

					_out = phaseModuleSignal(_type, _ts, _te, _tr, _amp, _freq, _fase);

                   if(size(data( _out )) == 1) _out =  _zeroSig;  
				   return( _out );
				break;

				case default 
					return ( _zeroSig );	

			}
		}
		else
		{
			return ( _zeroSig );
		}
	}	
}