/* $Id: WaveformListener.java,v 1.3 2002/07/26 10:56:02 manduchi Exp $ */
import java.util.EventListener;

public interface WaveformListener extends EventListener 
{
    public void processWaveformEvent(WaveformEvent e);
}
