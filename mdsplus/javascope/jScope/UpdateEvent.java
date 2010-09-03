package jScope;

/* $Id: UpdateEvent.java,v 1.1 2010/09/03 09:58:18 manduchi Exp $ */
import java.awt.AWTEvent;
import java.awt.Event;

/**
 * An UpdateEvent instance is passed to UpdateEventListener implementations as argument of processUpdateEvent
 * in order to notify the receipt of an asynchronous event triggering the display of a waveform.
 * 
 * @see UpdateEventListener
 * @see DataProvider
 */
public class UpdateEvent extends AWTEvent 
{
    /**
     * The name of the event just received.
     */
    String name;
    
    public UpdateEvent(Object source, String event) {
        super(source, 0);
        name = event;
    }
}
