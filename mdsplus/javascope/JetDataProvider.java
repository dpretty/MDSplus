import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.lang.InterruptedException;

class JetDataProvider implements DataProvider
{

    String provider;
    String experiment;
    int shot;
    String username, passwd;
    String encoded_credentials;
    String error_string;
    URL url;
    byte []buffer;
    Base64 translator = new Base64();
    BufferedInputStream br;
    int content_len;
    private String last_url_name;
    private float [] last_y, last_x, last_xdata;
    JDialog inquiry_dialog;
    JFrame owner_f;
    static final int LOGIN_OK = 1, LOGIN_ERROR = 2, LOGIN_CANCEL = 3;
    private int login_status;
    private boolean evaluate_url = false;
    private String url_source = "http://data.jet.uk/";

    private boolean use_cache = false;
    private SignalCache sc = null;
    private   Vector    connection_listener = new Vector();

    JTextField user_text; 
    JPasswordField passwd_text;

    JetDataProvider() {this(null, null);}

    JetDataProvider(String username, String passwd)
    {
        provider = "Jet Data";
        String credentials = username+":"+passwd;
        try{
            encoded_credentials = translator.encode(credentials);
        }catch(Exception e){}
    }

    public int GetLoginStatus()
    {
        return login_status;
    }

    public void setEvaluateUrl(boolean state)
    {
        evaluate_url = state;
    }

    public void setUrlSource(String url_source)
    {
        this.url_source = url_source;
        //System.out.println(url_source);
    }

    public boolean supportsCompression(){return false;}
    public void    setCompression(boolean state){}
    public boolean useCompression(){return false;}

    public boolean supportsCache(){return true;}
    public void    enableCache(boolean state)
    {
        if(state)
        {
            if(sc == null)
                sc = new SignalCache();
        } else
            sc = null;
        use_cache = state;
    }
    public boolean isCacheEnabled(){return use_cache;}
    public void    freeCache()
    {
        if(sc != null)
            sc.freeCache();
    }


    synchronized void InquireCredentials(JFrame f)
    {
        if(login_status == LOGIN_CANCEL)
        {
            login_status = LOGIN_OK;
            return;
        }
        login_status = LOGIN_OK;
        owner_f = f;
        inquiry_dialog = new JDialog(f, "JET data server login", true);

        inquiry_dialog.getContentPane().setLayout(new BorderLayout());
        JPanel p = new JPanel();
        p.add(new JLabel("Username: "));
        user_text = new JTextField(15);
        p.add(user_text);
        inquiry_dialog.getContentPane().add(p, "North");
        p = new JPanel();
        p.add(new JLabel("Password: "));
        passwd_text = new JPasswordField(15);
        passwd_text.setEchoChar('*');
        p.add(passwd_text);
        inquiry_dialog.getContentPane().add(p, "Center");
        p = new JPanel();
        JButton ok_b = new JButton("Ok");
        ok_b.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
                {
                    username = user_text.getText();
                    passwd = new String(passwd_text.getPassword());
                    if(!checkPasswd(username, passwd))
                    {
                        inquiry_dialog.setTitle("JET data server login ERROR");
//da mettere exception  ErrorMessage.ShowErrorMessage(owner_f, "Incorrect password");
                        login_status = LOGIN_ERROR;
                        return;
                    }

                    String credentials = username+":"+passwd;
                    try{
                        encoded_credentials = translator.encode(credentials);
                    } catch(Exception exc)
                    {
                        login_status = LOGIN_ERROR;
                    }
                    if(login_status == LOGIN_OK)
                        inquiry_dialog.setVisible(false);
                    else
                        login_status = LOGIN_OK;
                }});
        p.add(ok_b);
        JButton clear_b = new JButton("Clear");
        clear_b.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
                {
                    user_text.setText("");
                    passwd_text.setText("");
                }});
        p.add(clear_b);
        JButton cancel_b = new JButton("Cancel");
        cancel_b.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
                {
                    login_status = LOGIN_CANCEL;
                    inquiry_dialog.setVisible(false);
                }});
        p.add(cancel_b);
        inquiry_dialog.getContentPane().add(p, "South");
        inquiry_dialog.pack();
        Rectangle r = f.getBounds();
        inquiry_dialog.setLocation(r.x + r.width/2 - inquiry_dialog.getBounds().width/2,
			r.y + r.height/2 - inquiry_dialog.getBounds().height/2);
        inquiry_dialog.show();
    }

   public boolean checkPasswd(String encoded_credentials)
   {
            this.encoded_credentials = encoded_credentials;
            //System.out.println(encoded_credentials);
            try{
            URLConnection urlcon;
            url = new URL(url_source);
            urlcon = url.openConnection();
            urlcon.setRequestProperty("Authorization", "Basic "+encoded_credentials);
            br = new BufferedInputStream(urlcon.getInputStream());
            content_len = urlcon.getContentLength();
            if(content_len <= 0) return false;
            buffer = new byte[content_len];
            int num_read_bytes = 0;
            while(num_read_bytes < content_len)
                num_read_bytes += br.read(buffer, num_read_bytes, buffer.length - num_read_bytes);
            br.close();
            br = null;
        }catch(Exception e){System.out.println(e);return false;}
        String out = new String(buffer);
        if(out.indexOf("incorrect password") != -1)
            return false;
        return true;
   }


   boolean checkPasswd(String username, String passwd)
   {
        String credentials = username+":"+passwd;
        encoded_credentials = translator.encode(credentials);
        return checkPasswd(encoded_credentials);
   }
//DataProvider implementation

    public String GetDefaultTitle(String in_y[]){return null;}
    public String GetDefaultXLabel(String in_y[]){return null;}
    public String GetDefaultYLabel(String in_y[]){return null;}
    public float[] GetFrameTimes(String in_frame){return null;}
    public byte[] GetFrameAt(String in_frame, int frame_idx){return null;}
    public byte[]  GetAllFrames(String in_frame){return null;}
    
    public void SetEnvironment(String s) {}
    public void SetDefaultNid(String s) {}
    public void disconnect(){}
    
    public void Update(String experiment, int shot)
    {
        this.experiment = experiment;
        this.shot = shot;
        error_string = null;
    }
    public String GetString(String in) {return in; }
    public float GetFloat(String in){ return new Float(in).floatValue(); }

    public float[] GetFloatArray(String in_x, String in_y, float start, float end) throws IOException
    {
        return GetFloatArray(in_x);
    }

    public float[] GetFloatArray(String in) throws IOException
    {
        float out[] = null;
        String in_expr = new String(in);
        
        error_string = null;
        boolean is_time = in.startsWith("TIME:", 0);
        boolean is_xdata = in.startsWith("X:", 0);
        if(is_time)
            in = in.substring(5);
        if(is_xdata)
            in = in.substring(2);

        String url_name;

        if(evaluate_url)
        {
            url_name = in;
        }
        else
        {
            if(experiment == null)
            {
                StringTokenizer st = new StringTokenizer(in, "/", true);
                url_name = st.nextToken() + "/" + shot ;
                while(st.hasMoreTokens())
                    url_name = url_name + st.nextToken();
            }
            else
                url_name = experiment + "/" + shot + "/" + in;
        }

        out = null;
        if(use_cache)
        {
            String data_from = "Cache";
            out = (float[])sc.getCacheData(provider, in_expr, experiment, shot);
            if(out == null)
                data_from = "Network";
            ConnectionEvent e = new ConnectionEvent(this, data_from);
            dispatchConnectionEvent(e);

        } else {
            ConnectionEvent e = new ConnectionEvent(this, "Network");
            dispatchConnectionEvent(e);
        }
        
        if((last_url_name != null && url_name.equals(last_url_name)) || out!= null)
        {
            if(out != null)
                return out;
                
            if(is_time)
                return last_x;
            else
                if(is_xdata)
                    return last_xdata;
                else
                    return last_y;
        }
        else
        {
            last_x = last_y = last_xdata = null;
            try{
                last_url_name = url_name;
                URLConnection urlcon;
                url = new URL(url_source + url_name);
                urlcon = url.openConnection();
                //urlcon.setRequestProperty("Connection", "Keep-Alive");
                urlcon.setRequestProperty("Authorization", "Basic "+encoded_credentials);
                InputStream is = urlcon.getInputStream();
                br = new BufferedInputStream(is);
                content_len = urlcon.getContentLength();
                if(content_len <= 0)
                {
                    last_url_name = null;
                    error_string = "Error reading URL " + url_name + " : null content length";
                    throw(new IOException(error_string));
                    //return null;
                }
                buffer = new byte[content_len];
                int num_read_bytes = 0;
                while(num_read_bytes < content_len)
                    num_read_bytes += br.read(buffer, num_read_bytes, buffer.length - num_read_bytes);
                br.close();
                br = null;

                JiNcSource jns = new JiNcSource("myname", new RandomAccessData(buffer));

                JiVar jvarData = jns.getVar("SIGNAL");
                int ndims = jvarData.getDims().length;
                JiDim jdimTime = jvarData.getDims()[ndims-1];
                JiVar jvarTime = jns.getVar(jdimTime.mName);


                JiDim jdimXData = null;
                JiVar jvarXData = null;
                if (ndims >= 2){
                    jdimXData = jvarData.getDims()[ndims-2];
                    if (jdimXData != null){
                        jvarXData = jns.getVar(jdimXData.mName);
                    }
                }

                JiDim[] dims = jvarTime.getDims();
                double[] time = jvarTime.readDouble(dims);
                last_x = new float[time.length];
                for(int i = 0; i < time.length; i++)
                    last_x[i] = (float)time[i];
                time = null;

                dims = jvarData.getDims();
                last_y = jvarData.readFloat(dims);

                if(jvarXData != null)
                {
                    dims = jvarXData.getDims();
                    last_xdata = jvarXData.readFloat(dims);
                }

            }catch(Exception e)
            {
                error_string = "Error reading URL " + url_name + " : " + e;
                last_url_name = null;
                throw(new IOException(error_string));
//                return null;
            }



            if(use_cache) 
            {
                sc.putCacheData(provider, in, experiment, shot, last_y);
                sc.putCacheData(provider, "TIME:"+in, experiment, shot, last_x);
                if(last_xdata != null)
                    sc.putCacheData(provider, "X:"+in, experiment, shot, last_xdata);
            }
            
            if(is_time)            
                return last_x;
             else 
                if(is_xdata)
                    return last_xdata;
                else
                    return last_y;

        }
    }

 public int[] GetIntArray(String in) throws IOException
 {
    error_string = null;
    int [] result;
    String curr_in = in.trim();
    if(curr_in.startsWith("[", 0))
    {
        if(curr_in.endsWith("]"))
        {
            curr_in = curr_in.substring(1, curr_in.length() - 1);
            StringTokenizer st = new StringTokenizer(curr_in, ",", false);
            result = new int[st.countTokens()];
            int i = 0;
            try{
                while(st.hasMoreTokens())
                    result[i++] = Integer.parseInt(st.nextToken());
                return result;
            } catch(Exception e) {}
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
                try{
                    start = Integer.parseInt(st.nextToken());
                    end = Integer.parseInt(st.nextToken());
                    if(end < start) end = start;
                    result = new int[end-start+1];
                    for(int i = 0; i < end-start+1; i++)
                        result[i] = start+i;
                    return result;
                }catch(Exception e){}
            }
        }
        else
        {
            result = new int[1];
            try {
                result[0] = Integer.parseInt(curr_in);
                return result;
            }catch(Exception e){}
        }
    }
    error_string = "Error parsing shot number(s)";
    throw(new IOException(error_string));
    //return null;
 }

 public String GetXSpecification(String in)
 {
    return "TIME:" + in;
 }

 public String GetXDataSpecification(String in)
 {
    return "X:" + in;
 }


 public String ErrorString() { return error_string; }
 public boolean SupportsAsynch() { return false; }
 public void addNetworkListener(NetworkListener l, String event){}
 public void removeNetworkListener(NetworkListener l, String event){}

 public void addConnectionListener(ConnectionListener l)
 {
	if (l == null) {
	    return;
	}
    connection_listener.addElement(l);
 }
 
 public void removeConnectionListener(ConnectionListener l)
 {
	if (l == null) {
	    return;
	}
    connection_listener.removeElement(l);
 }
 
    protected void dispatchConnectionEvent(ConnectionEvent e) 
    {
        if (connection_listener != null) 
        {
            for(int i = 0; i < connection_listener.size(); i++)
            {
                ((ConnectionListener)connection_listener.elementAt(i)).processConnectionEvent(e);
            }
        }
    }



public static void main(String args[])
{
    System.out.println("\nStart readout PPF/40573/MAGN/IPLA");
    JetDataProvider dp = new JetDataProvider("obarana", "clublatino");
    dp.setEvaluateUrl(true);
    float x[], y[], xdata[];
    try
    {
    y = dp.GetFloatArray("PPF/40573/MAGN/BPOL");
    x = dp.GetFloatArray("TIME:PPF/40573/MAGN/BPOL");
    xdata = dp.GetFloatArray("X:PPF/40573/MAGN/BPOL");

    for(int i = 0; i < x.length; i++)
        System.out.println(x[i] + "  " +y[i]);

    System.out.println("Num. points: "+y.length);
    } catch (IOException exc){}
 }

}

