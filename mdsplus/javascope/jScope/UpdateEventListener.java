package jScope;

/* $Id: UpdateEventListener.java,v 1.1 2010/09/03 09:58:18 manduchi Exp $ */
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
