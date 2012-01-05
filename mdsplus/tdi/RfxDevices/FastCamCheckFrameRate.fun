public fun FastCamCheckFrameRate(in _resolution )
{
																							
	_fRateTable = [ 1000, 2000, 3000,  6000,  10000, 27000, 45000, 
				  3000, 6000, 10000, 18000, 27000, 45000, 
				  2000, 3000, 10000, 18000, 27000, 45000, 73000, 
				  3000, 6000, 10000, 27000, 45000, 73000, 109500 ];

	_string = ["1024x1024","1024x512","1024x256","1024x128","1024x64","1024x32","1024x16", 
			   "512x512"  ,"512x256" ,"512x128" ,"512x64"  ,"512x32" ,"512x16", 
			   "256x1024" ,"256x512" ,"256x256" ,"256x128" ,"256x64" ,"256x32" ,"256x16", 
			   "128x1024" ,"128x512" ,"128x256" ,"128x128" ,"128x64" ,"128x32" ,"128x16"];


	for( _i = 0; _i < 27; _i++)
	{
		if( _resolution == _string[_i] )
		{
			write(*, "Resolution "//_resolution//" Max frame rate "//trim(adjustl(_fRateTable[ _i ])));
			return ( _fRateTable[ _i ] );
		}
	}
	return (0);
}

