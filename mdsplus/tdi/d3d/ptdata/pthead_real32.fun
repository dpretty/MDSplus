FUN PUBLIC PTHEAD_REAL32(IN _pointname, OPTIONAL IN _shot, OPTIONAL OUT _error) { 

        IF (NOT PRESENT(_shot)) _shot=$SHOT;
        _error=0;
        _ifix = pthead2(_pointname,_shot,_error);

        /* Skip first two elements to match old ptdata */
        return(__real32[2:*]);

}
	
