public fun DIO2__dw_setup(as_is _nid, optional in _method)
{
  _head = DevHead(_nid);  
  _path=getnci(_head, "FULLPATH");
  _tree = getdbi("NAME");
  JavaMds->deviceSetup("DIO2",_tree, val($shot),_path,val(20),val(20)) ;
  return(1);
}
