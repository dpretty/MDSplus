public fun FastCamHWinit(in _nRecordRate, in _numFrame, in _numTrigger, in _nShutterSpeed, in _vRes, in _hRes, in _TrigMode)
{
	return ( FastCam->FastCamInit(val(_nRecordRate) , val(_numFrame) ,val(_numTrigger), val(_nShutterSpeed) ,val(_vRes) ,val(_hRes) ,val(_TrigMode)) ) ;
}

