package jScope;

/* $Id: WaveformListener.java,v 1.1 2010/09/03 09:58:18 manduchi Exp $ */
import java.util.EventListener;

public interface WaveformListener extends EventListener 
{
    public void processWaveformEvent(WaveformEvent e);
}
