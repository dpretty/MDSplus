public fun iseps_finish_pulse()
{
    write(*, 'ISEPS FINISH PULSE');
    tcl('dispatch/command/server=localhost:8001 dispatch/phase finish_pulse');
}
