/* $Id: DataAccessURL.java,v 1.20 2003/06/10 12:23:20 manduchi Exp $ */
import java.util.Vector;
import java.io.IOException;

class DataAccessURL  
{
    
    static Vector dataAccessVector = new Vector();
        
    static public void addProtocol(DataAccess dataAccess)
    {
        dataAccessVector.addElement(dataAccess);
    }

    static public Signal getSignal(String url) throws IOException
    {
        return getSignal(url, null, null);
    }

    static public Signal getSignal(String url, String passwd) throws IOException
    {
        return getSignal(url, null, passwd);
    }
    
    static public DataAccess getDataAccess(String url) throws IOException
    {
        DataAccess da = null;
        
        for(int i = 0 ; i < dataAccessVector.size(); da = null, i++)
        {
            da = (DataAccess)dataAccessVector.elementAt(i);
            if(da.supports(url))
                break;
        }
        if(da == null)
            throw(new IOException("Protocol not recognized"));       
        return da;
    }
    
    static public Signal getSignal(String url, String name, String passwd) throws IOException
    {
        DataAccess da = null;
        
        if((da = getDataAccess(url)) != null)
        {
            da.setPassword(passwd);
            Signal s = da.getSignal(url);
            if(s == null && da.getError() == null)
                throw(new IOException("Incorrect password or read signal error"));
            
            if(da.getError() == null)
            {
                if(name == null)
                    name = s.getName();
                    
                if(name == null)
                    name = da.getSignal()+" "+da.getShot();
                else
                    name = name+" "+da.getShot();                
                s.setName(name);
                return s;
            }
            else
            {
                throw(new IOException(da.getError()));
            }
        }
        return null;
    }    

    static public Frames getImages(String url, Frames f) throws IOException
    {
        return getImages(url, null, null, f);
    }

    static public Frames getImages(String url, String passwd, Frames f) throws IOException
    {
        return getImages(url, null, passwd, f);
    }

    static public Frames getImages(String url, String name, String passwd, Frames f) throws IOException
    {
        DataAccess da = null;
        
        if((da = getDataAccess(url)) != null)
        {
            da.setPassword(passwd);
            f = da.getImages(url, f);
            if(f == null && da.getError() == null)
                throw(new IOException("Incorrect password or read images error"));
            
            if(da.getError() == null)
            {
                return f;
            }
            else
            {
                throw(new IOException(da.getError()));
            }
        }
        throw(new IOException("Protocol not recognized"));
    }
    
    
    static public void close()
    {
        DataAccess da = null;
        for(int i = 0 ; i < dataAccessVector.size(); i++)
        {
            da = (DataAccess)dataAccessVector.elementAt(i);
            if(da != null)
                da.close();
        }
    }

    static public int getNumProtocols()
    {
        return dataAccessVector.size();
    }

}
