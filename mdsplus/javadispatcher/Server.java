interface Server
/**
Abstracts Server functionality
*/
{

    public void pushAction(Action action);
    /** Inserts a new action in the waiting queue 
    */
    public Action popAction();
    /** Removes the lastly inserted action form the waiting queue. 
    If no such action is found return null
    */
    
    public Action[] collectActions();
    /** Collects action information to be used by the dispatcher
    */
    public void beginSequence(int shot);
    /** Handles the beginning of a sequence (e.g. for opening trees)
    */
    public void endSequence(int shot);
    /** Handles the termination of the sequence (e.g. for closing trees)
    */
    public void addServerListener(ServerListener listener);
    /** Add a new listener for actions handled by the server
    */
    public String getServerClass();
    /**
    returns the classes to which the server belongs
    */
    public int getQueueLength();
    /**
    returns the number of waiting actions
    */
    public void abort(boolean flush);
    /**
    aborts action currently being executed. If flush, empties waiting queue;
    */
    public boolean abortAction(Action action);
    /**
    aborts selected action
    */
    
    public boolean isActive();
    public void setTree(String tree);
  
    
}  
    
    