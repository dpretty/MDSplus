/* $Id: WaveContainerListener.java,v 1.2 2002/04/26 13:14:30 jgk Exp $ */
import java.util.EventListener;

public interface WaveContainerListener extends EventListener 
{
    public void processWaveContainerEvent(WaveContainerEvent e);
}
