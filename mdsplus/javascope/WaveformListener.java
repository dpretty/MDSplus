/* $Id: WaveformListener.java,v 1.2 2002/04/26 13:14:30 jgk Exp $ */
import java.util.EventListener;

public interface WaveformListener extends EventListener 
{
    public void processWaveformEvent(WaveformEvent e);
}
