package jScope;

/* $Id: WaveContainerListener.java,v 1.1 2010/09/03 09:58:18 manduchi Exp $ */
import java.util.EventListener;

public interface WaveContainerListener extends EventListener 
{
    public void processWaveContainerEvent(WaveContainerEvent e);
}
