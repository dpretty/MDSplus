public fun DT_ACQ16__PART_NAME(as_is _nid, optional in _method)
{
  _name = ([
  '',
  ":NODE",
  ":BOARD",
  ":COMMENT",
  ":DI0",
  ":DI0:WIRE",
  ":DI0:BUS",
  ":DI1",
  ":DI1:WIRE",
  ":DI1:BUS",
  ":DI2",
  ":DI2:WIRE",
  ":DI2:BUS",
  ":DI3",
  ":DI3:WIRE",
  ":DI3:BUS",
  ":DI4",
  ":DI4:WIRE",
  ":DI4:BUS",
  ":DI5",
  ":DI5:WIRE",
  ":DI5:BUS",
  ":CLOCK_SRC",
  ":CLOCK_DIV",
  ":DAQ_MEM",
  ":ACTIVE_CHAN",
  ":TRIG_SRC",
  ":POST_TRIG",
  ":PRE_TRIG",
  ":INPUT_01",
  ":INPUT_01:STARTIDX",
  ":INPUT_01:ENDIDX",
  ":INPUT_01:INC",
  ":INPUT_01:FILTER_COEFS",
  ":INPUT_02",
  ":INPUT_02:STARTIDX",
  ":INPUT_02:ENDIDX",
  ":INPUT_02:INC",
  ":INPUT_02:FILTER_COEFS",
  ":INPUT_03",
  ":INPUT_03:STARTIDX",
  ":INPUT_03:ENDIDX",
  ":INPUT_03:INC",
  ":INPUT_03:FILTER_COEFS",
  ":INPUT_04",
  ":INPUT_04:STARTIDX",
  ":INPUT_04:ENDIDX",
  ":INPUT_04:INC",
  ":INPUT_04:FILTER_COEFS",
  ":INPUT_05",
  ":INPUT_05:STARTIDX",
  ":INPUT_05:ENDIDX",
  ":INPUT_05:INC",
  ":INPUT_05:FILTER_COEFS",
  ":INPUT_06",
  ":INPUT_06:STARTIDX",
  ":INPUT_06:ENDIDX",
  ":INPUT_06:INC",
  ":INPUT_06:FILTER_COEFS",
  ":INPUT_07",
  ":INPUT_07:STARTIDX",
  ":INPUT_07:ENDIDX",
  ":INPUT_07:INC",
  ":INPUT_07:FILTER_COEFS",
  ":INPUT_08",
  ":INPUT_08:STARTIDX",
  ":INPUT_08:ENDIDX",
  ":INPUT_08:INC",
  ":INPUT_08:FILTER_COEFS",
  ":INPUT_09",
  ":INPUT_09:STARTIDX",
  ":INPUT_09:ENDIDX",
  ":INPUT_09:INC",
  ":INPUT_09:FILTER_COEFS",
  ":INPUT_10",
  ":INPUT_10:STARTIDX",
  ":INPUT_10:ENDIDX",
  ":INPUT_10:INC",
  ":INPUT_10:FILTER_COEFS",
  ":INPUT_11",
  ":INPUT_11:STARTIDX",
  ":INPUT_11:ENDIDX",
  ":INPUT_11:INC",
  ":INPUT_11:FILTER_COEFS",
  ":INPUT_12",
  ":INPUT_12:STARTIDX",
  ":INPUT_12:ENDIDX",
  ":INPUT_12:INC",
  ":INPUT_12:FILTER_COEFS",
  ":INPUT_13",
  ":INPUT_13:STARTIDX",
  ":INPUT_13:ENDIDX",
  ":INPUT_13:INC",
  ":INPUT_13:FILTER_COEFS",
  ":INPUT_14",
  ":INPUT_14:STARTIDX",
  ":INPUT_14:ENDIDX",
  ":INPUT_14:INC",
  ":INPUT_14:FILTER_COEFS",
  ":INPUT_15",
  ":INPUT_15:STARTIDX",
  ":INPUT_15:ENDIDX",
  ":INPUT_15:INC",
  ":INPUT_15:FILTER_COEFS",
  ":INPUT_16",
  ":INPUT_16:STARTIDX",
  ":INPUT_16:ENDIDX",
  ":INPUT_16:INC",
  ":INPUT_16:FILTER_COEFS",
  ":INIT_ACTION",
  ":STORE_ACTION"])[getnci(_nid,'conglomerate_elt')-1];
  return(trim(_name));
}