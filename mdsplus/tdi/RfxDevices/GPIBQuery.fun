public fun GPIBQuery(in _ud, in _command, in _size)
{

write(*, 'Sono la GPIBQuery');
  return(gpiblib->GPIBQuery(_ud, _command, _size));
}
