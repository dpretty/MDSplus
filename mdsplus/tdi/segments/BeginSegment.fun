public fun BeginSegment(as_is _node, in _start, in _end, as_is _dim, in _array, optional _idx) {
  _nid=getnci(_node,"NID_NUMBER");
  if (!present(_idx)) {
    _idx=-1;
  }
  return(TreeShr->TreeBeginSegment(val(_nid),descr(_start),descr(_end),xd(_dim),descr(data(_array)),val(_idx)));
}
