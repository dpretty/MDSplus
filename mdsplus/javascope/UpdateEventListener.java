/* $Id: UpdateEventListener.java,v 1.2 2002/04/26 13:14:30 jgk Exp $ */
import java.util.EventListener;

/**
 * An implementation of UpdateEventListener is passed in DataProvider.addUpdateEventListener method, to
 * be notified by the DataProvider implementation when the corresponding asynchronous event is received.
 */
public interface UpdateEventListener extends EventListener 
{
    /**
     * Called by the DataProvider implementation to notify jScope that an asynchronous event has been received.
     * 
     * @param e The UpdateEvent describing the event (i.e. its name) just occured.
     * @see UpdateEvent
     * @see DataProvider
     */
    public void processUpdateEvent(UpdateEvent e);

}
