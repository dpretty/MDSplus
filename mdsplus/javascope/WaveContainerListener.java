/* $Id: WaveContainerListener.java,v 1.3 2002/07/26 10:56:01 manduchi Exp $ */
import java.util.EventListener;

public interface WaveContainerListener extends EventListener 
{
    public void processWaveContainerEvent(WaveContainerEvent e);
}
