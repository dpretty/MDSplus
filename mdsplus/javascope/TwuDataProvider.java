// -------------------------------------------------------------------------------------------------
// TwuDataProvider
// An implementation of "DataProvider" for signals from a TEC Web-Umbrella (TWU) server.
//
// The first versions of this class (cvs revisions 1.x) were designed and written
// by Gabriele Manduchi and with some minor hacks by Jon Krom.   
// Marco van de Giessen <A.P.M.vandeGiessen@phys.uu.nl> did some major surgery on
// this class (starting revision line 2.x) mainly in order to ensure that zooming
// works in more situations. (See also the cvs log.)
//
// Most of this revision 2.x work has now, from 2.21 onwards, been moved into separate classes.
// No new major revision number was started; has little use in CVS.
//
// $Id: TwuDataProvider.java,v 2.27 2003/10/21 15:50:59 jgk Exp $
// 
// -------------------------------------------------------------------------------------------------

import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.lang.InterruptedException;


class TwuDataProvider
    implements DataProvider
{
    protected String provider_url = "ipptwu.ipp.kfa-juelich.de";
    protected String experiment;
    protected long   shot;
    protected String error_string;
    private transient Vector   connection_listener = new Vector();
    private String user_agent;
    private TwuWaveData lastWaveData = null ;


    //DataProvider implementation
    public boolean SupportsCompression(){return false;}
    public void    SetCompression(boolean state){}
    public void    SetEnvironment(String s) {}
    public void    Dispose(){}
    public String  GetString(String in) {return in; }
    public float   GetFloat(String in){ return new Float(in).floatValue(); }
    public String  ErrorString() { return error_string; }
    public void    AddUpdateEventListener   (UpdateEventListener l, String event){}
    public void    RemoveUpdateEventListener(UpdateEventListener l, String event){}
    public boolean SupportsContinuous() {return false; }
    public boolean DataPending()        {return false; }
    public int     InquireCredentials(JFrame f, DataServerItem server_item){return DataProvider.LOGIN_OK;}
    public boolean SupportsFastNetwork(){return true;}
    public void    SetArgument(String arg){}
    public boolean SupportsTunneling() {return false;}

    //  --------------------------------------------------------------------------------------------
    //     interface methods for getting *Data objects
    //  ---------------------------------------------------

    public FrameData GetFrameData(String in_y, String in_x, float time_min, float time_max) 
        throws IOException
    {
        return (new TwuSimpleFrameData(this, in_y, in_x, time_min, time_max));
    }

    //  ---------------------------------------------------
    public synchronized WaveData GetWaveData (String in) 
    { 
        return GetWaveData (in, null); 
    }

    public synchronized WaveData GetWaveData (String in_y, String in_x) 
    {
        TwuWaveData find = FindWaveData (in_y, in_x);
        find.setFullFetch() ;
        return find ;
    }

    public synchronized WaveData GetResampledWaveData(String in, float start, float end, int n_points) 
    { 
        return GetResampledWaveData(in, null, start, end, n_points); 
    }

    public synchronized WaveData
    GetResampledWaveData(String in_y, String in_x, float start, float end, int n_points) 
    {
        TwuWaveData find = FindWaveData (in_y, in_x);
        find.setZoom (start, end, n_points);
        return find ;
    }

    public synchronized TwuWaveData FindWaveData (String in_y, String in_x) 
    {
        if ( lastWaveData == null  ||  lastWaveData.notEqualsInputSignal (in_y, in_x, shot) )
        {
            lastWaveData = new TwuWaveData (this, in_y, in_x) ;
            try
            {
                // Ensure that the properties are fetched right away.
                lastWaveData.getTWUProperties() ;
            }
            catch (IOException e)
            {
                if (error_string==null)
                  error_string = "No Such Signal : " + GetSignalPath(in_y, shot, provider_url, experiment) ;
                //throw new IOException ("No Such Signal");
            }
        }
        return lastWaveData ;
    }

    //  ----------------------------------------------------------
    //     abscissa / signal properties / path utility methods
    //  ----------------------------------------------------------

    static
    protected synchronized String 
    GetSignalPath(String in, long shot, String provider_url, String experiment)
    {
        if(IsFullURL(in))
          return in;
        else
        {
            // Hashed_URLs
            // Check if signal path is in the format
            //   //url_server_address/experiment/shotGroup/#####/signal_path

            if(TwuNameServices.isHashedURL(in))
              return TwuNameServices.hashed2shot(in,shot);

            // If not, then it is of the old jScope internal format 
            //   url_server_address//group/signal_path
            // (Continue handling them; they could come out of .jscp files)
            
            String p_url = GetURLserver(in);
            if(p_url == null)
              p_url = provider_url;
            else
              in = in.substring(in.indexOf("//")+2, in.length());

            StringTokenizer st = new StringTokenizer(in, "/");
            String full_url = "http://"+p_url+"/"+experiment+"/"+st.nextToken()+
                "/"+shot;
            while(st.hasMoreTokens())
              full_url += "/"+st.nextToken();
            return full_url;
        }
    }

    static
    private boolean IsFullURL(String in)
    {
        in = in.toLowerCase();
        return (in.startsWith("http://") || in.startsWith("//") ) && in.indexOf("#")==-1 ;
    }

    static 
    private String GetURLserver(String in)
    { 
        // Find the servername, if it follows the (early) jScope internal
        // convention that it is encoded before the double slash.
        int idx;
        String out = null;
        if((idx = in.indexOf("//")) != -1)
          out = in.substring(0, idx);

        return out;
    }

    // ---------------------------------------------------------------------------------------------

    public synchronized float[] GetFloatArray(String in)
    {
        boolean is_time;
        error_string = null;
        if(in.startsWith("TIME:", 0))
        {
            is_time = true;
            in = in.substring(5);
        }
        else
          is_time = false;

        WaveData wd   = GetWaveData  (in) ;
        float [] data = null ;
        try
        {  
            data = wd.GetFloatData() ; 
        }
        catch (  IOException e ) 
        { 
            error_string = e.toString() ; 
            data = null ; 
        }
        return data ;
    }

    //  ----------------------------------------------------
    //  Methods for TwuAccess.
    //  ----------------------------------------------------

    public synchronized float[] 
    GetFloatArray (String in, boolean is_time) throws IOException
    {
        WaveData wd = GetWaveData(in) ; // TwuAccess wants to get the full signal data .
        return is_time ? wd.GetXData() : wd.GetYData() ;
    }

    public synchronized String GetSignalProperty (String prop, String in) throws IOException
    {
        TwuWaveData wd = (TwuWaveData) GetWaveData(in) ;  
        return wd.getTWUProperties().getProperty(prop);
    }

    //  -------------------------------------------------------
    //     parsing of / extraction from input signal string
    //  -------------------------------------------------------

    public long[] GetShots(String in)
    {
        error_string = null;
        long [] result;
        String curr_in = in.trim();
        if(curr_in.startsWith("[", 0))
        {
            if(curr_in.endsWith("]"))
            {
                curr_in = curr_in.substring(1, curr_in.length() - 1);
                StringTokenizer st = new StringTokenizer(curr_in, ",", false);
                result = new long[st.countTokens()];
                int i = 0;
                try
                {
                    while(st.hasMoreTokens())
                      result[i++] = Integer.parseInt(st.nextToken());
                    return result;
                } 
                catch(Exception e) {}
            }
        }
        else
        {
            if(curr_in.indexOf(":") != -1)
            {
                StringTokenizer st = new StringTokenizer(curr_in, ":");
                int start, end;
                if(st.countTokens() == 2)
                {
                    try
                    {
                        start = Integer.parseInt(st.nextToken());
                        end = Integer.parseInt(st.nextToken());
                        if(end < start) 
                          end = start;
                        result = new long[end-start+1];
                        for(int i = 0; i < end-start+1; i++)
                          result[i] = start+i;
                        return result;
                    }
                    catch(Exception e){}
                }
            }
            else
            {
                result = new long[1];
                try 
                {
                    result[0] = Long.parseLong(curr_in);
                    return result;
                }
                catch(Exception e){}
            }
        }
        error_string = "Error parsing shot number(s)";
        return null;
    }

    //  -------------------------------------------
    //     connection handling methods ....
    //  -------------------------------------------

    public synchronized void AddConnectionListener(ConnectionListener l) 
    {
        if (l == null) 
          return;

        connection_listener.addElement(l);
    }

    public synchronized void RemoveConnectionListener(ConnectionListener l) 
    {
        if (l == null)
          return;

        connection_listener.removeElement(l);
    }

    protected void DispatchConnectionEvent(ConnectionEvent e) 
    {
        if (connection_listener != null) 
        {
            for(int i = 0; i < connection_listener.size(); i++)
              ((ConnectionListener)connection_listener.elementAt(i)).processConnectionEvent(e);
        }
    }

    //  -------------------------------------------
    //  Constructor, other small stuff ...
    //  -------------------------------------------

    public synchronized void 
    Update(String experiment, long shot)
    {
        this.experiment = experiment;
        this.shot = shot;
        error_string = null;
        lastWaveData = null;
    }

    public TwuDataProvider()
    {
        super();
    }

    public static String 
    revision()
    {
        return "$Id: TwuDataProvider.java,v 2.27 2003/10/21 15:50:59 jgk Exp $";
    }

    public TwuDataProvider(String user_agent)
    {
        this.user_agent = user_agent;
        // Could be used in the constructor for TWUProperties and in similar get URL actions.
        // A site could used this as a possible (internal) software distribution management
        // tool.  In the log of a web-server you can, by checking the user_agent, see which
        // machines are still running old software.
    }
}

// -------------------------------------------------------------------------------------------------
// End of: $Id: TwuDataProvider.java,v 2.27 2003/10/21 15:50:59 jgk Exp $
// -------------------------------------------------------------------------------------------------
