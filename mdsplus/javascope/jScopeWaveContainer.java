import java.awt.Graphics;
import java.awt.Component;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.TextField;
import java.awt.Label;
import java.awt.Frame;
import java.awt.Point;
import java.awt.Cursor;
import java.awt.Color;
import java.awt.Container;
import java.awt.Component;
import java.awt.FileDialog;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.Date;
import java.io.*;
import javax.swing.*;
import java.awt.print.*;


class jScopeWaveContainer extends WaveformContainer 
{
    private   static final int MAX_COLUMN = 4;       

              DataProvider dp;
    private   jScopeDefaultValues def_vals;
    private   boolean   fast_network_access = false;
    private   boolean   supports_fast_network = false;
    private   boolean   supports_local = true;
    private   String    title = null;
    private   String    server_name = null;
    private   String    event = null;
    private   String    print_event = null;
    private   Color     colors[] = Waveform.COLOR_SET;
    private   String    colors_name[] = Waveform.COLOR_NAME;
    private   int	    grid_mode = 0, x_lines_grid = 3, y_lines_grid = 3;
    private   boolean   brief_error = true;
    private   int       columns;
    private   UpdW      updateThread;
    private   boolean   abort;
    private   boolean   add_sig = false;
    private   ProfileDialog profile_dialog;
    private   int       main_shots[] = null;
    private   String    main_shot_str = null;
    private   boolean   main_shot_changed = false;
    private   String    main_shot_error = null;
    private   jScopeMultiWave profile_source = null;
              MdsWaveInterface mds_wi[];
     

    class UpdW extends Thread  
    {
        boolean pending = false;
  
        synchronized public void run()
        {
                      
            Date date = new Date();
            long start, end;
            WaveContainerEvent wce = null;

              
            setName("Update Thread");

            while(true)
            {
                    
                try {
                            
                    if(!pending)
                        wait();
                    pending = false;


                    date = new Date();
                    start = date.getTime();
                            
                    wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, "Start Update");
                    jScopeWaveContainer.this.dispatchWaveContainerEvent(wce);
                    	    
                    try
                    {
                               
	                    UpdateAllWave();
                         	
                        date = new Date();
                        end = date.getTime();
                         	     	    
	                    if(!abort)
                                wce = new WaveContainerEvent(this, WaveContainerEvent.END_UPDATE, "All waveforms are up to date < "+(end-start)+" ms >");
	                    else
                                wce = new WaveContainerEvent(this, WaveContainerEvent.KILL_UPDATE, " Aborted ");

                        jScopeWaveContainer.this.dispatchWaveContainerEvent(wce);
                    	    	    
	                } 
	                catch(Throwable e) 
	                {
                        wce = new WaveContainerEvent(this, WaveContainerEvent.KILL_UPDATE, "Error during apply " + e);
                        jScopeWaveContainer.this.dispatchWaveContainerEvent(wce);
	                }
                } catch (InterruptedException e){}
            }
        }     
         
        synchronized public void StartUpdate()
        {
            pending = true;
            notify();
        }
        
    }

    public jScopeWaveContainer(int rows[], jScopeDefaultValues def_vals)
    {
        this(rows, null, def_vals);
    }

       
    public jScopeWaveContainer(int rows[], DataProvider dp, jScopeDefaultValues def_vals)
    {
        super(rows, false);
        this.def_vals = def_vals;
        this.dp = dp; 
        Component c[] = CreateWaveComponents(getComponentNumber());
        AddComponents(c);
        updateThread = new UpdW();
        updateThread.start();
        setBackground(Color.white);
    }
    
   protected Component[] CreateWaveComponents(int num)
   {
        Component c[] = new Component[num];
        jScopeMultiWave      wave = null;
        for(int i = 0; i < c.length;i++)
        {
	       wave = new jScopeMultiWave(dp, def_vals);
	       wave.wi.full_flag = !GetFastNetworkState();
	       wave.addWaveformListener(this);
	       SetWaveParams(wave);
           c[i] = wave;
        }
        return c;
    }
    
    public void initMdsWaveInterface()
    {
        jScopeMultiWave w;
        
        mds_wi = new MdsWaveInterface[getGridComponentCount()];
	    for(int i = 0, k = 0; i < 4; i++)
	    {
	        for(int j = 0; j < rows[i]; j++, k++) 
		    {
		        w = (jScopeMultiWave)getGridComponent(k);
		        if(w != null)
		            mds_wi[k] = (MdsWaveInterface)w.wi;
		    }
	    }
    }
    
    
    public void ChangeDataProvider(DataProvider dp)
    {
        jScopeMultiWave w;
                 
        main_shot_changed = true;
	    for(int i = 0; i < getGridComponentCount(); i++)
        {
	        w = (jScopeMultiWave)GetWavePanel(i);
	        if(w != null)
	        {
	   	        if(w.wi != null)
		            w.wi.SetDataProvider(dp);
	            w.Erase();
	            w.SetTitle(null);
		    }
		}
    }
    
    public void Reset()
    {
        int reset_rows[] = {1, 0, 0, 0};
        ph = null;
        pw = null;
        SetTitle(null);
        event = null;
        print_event = null;
        ResetDrawPanel(reset_rows);
        update();
        jScopeMultiWave w = (jScopeMultiWave)GetWavePanel(0);
        w.jScopeErase();
        def_vals.Reset();
    }

    public void NotifyChange(Waveform dest, Waveform source)    
    {
     
       jScopeMultiWave w =  ((jScopeMultiWave)source);
        
       MdsWaveInterface mwi = new MdsWaveInterface(w.wi);
       mwi.setDefaultsValues(def_vals);
	  ((jScopeMultiWave)dest).wi = mwi;
	  ((jScopeMultiWave)dest).wi.SetDataProvider(dp);
	  ((jScopeMultiWave)dest).wi.wave = dest;
	  Refresh((jScopeMultiWave)dest, "Copy in");
	  
    }

    public String GetTitle(){return title;}
    public String GetEvent(){return event;}
    public String GetPrintEvent(){return print_event;}
    public String GetServerName(){return server_name;}
    
    public void    SetTitle(String title){this.title = title;}
    public void    SetServerName(String server_name){this.server_name = server_name;}
    public boolean supportsFastNetwork(){return supports_fast_network;}
    public boolean supportsLocalProvider(){return supports_local;}
    
    public boolean supportsCompression()
    {
        if(dp != null)
            return dp.supportsCompression();
        else
            return false;
    }
    
    
    public boolean compressionEnable()
    {
        if(dp != null)
            return dp.useCompression();
        else
            return false;
    }

    public void setCompression(boolean state, NetworkEventListener l) throws IOException
    {
        if(dp != null && dp.supportsCompression() && dp.useCompression() != state) 
        {
            RemoveAllEvents(l);
            dp.disconnect();
            dp.setCompression(state);
            AddAllEvents(l);
        }
    }
    
    public void freeCache()
    {
        dp.freeCache();
    }

    
    public boolean supportsCache()
    {
        if(dp != null)
            return dp.supportsCache();
        else
            return false;
    }

    public void enableCache(boolean state)
    {
        if(dp != null)
            dp.enableCache(state);
    }

    public boolean isCacheEnabled()
    {
        if(dp != null)
            return dp.isCacheEnabled();
        else
            return false;
    }


    
  public void SetBriefError(boolean brief_error){this.brief_error = brief_error;}
    
  public void PrintAllWaves(PrinterJob prnJob, PageFormat pf) throws PrinterException
  {
       prnJob.setPrintable(this, pf);
       prnJob.print();
  }


    public void processWaveformEvent(WaveformEvent e) 
    {
        super.processWaveformEvent(e);
        jScopeMultiWave w = (jScopeMultiWave)e.getSource();
        switch(e.getID())
        {
            case WaveformEvent.PROFILE_UPDATE:
                if(profile_dialog != null && profile_dialog.isShowing())
                {
                    profile_dialog.updateProfiles(e.name,
                                                  e.pixels_x, e.start_pixel_x, 
                                                  e.pixels_y, e.start_pixel_y,
                                                  e.pixels_signal, e.frames_time);
                    if(e.pixels_line != null)
                        profile_dialog.updateProfileLine(e.pixels_line);
                    else
                        profile_dialog.removeProfileLine();
                    
                }
            return;
        }
    }

    public void ShowProfileDialog(jScopeMultiWave wave)
    {
        if(profile_dialog != null && profile_dialog.isVisible())
            profile_dialog.dispose();
        //if(profile_dialog == null)
        {
            profile_dialog = new ProfileDialog(wave);
            profile_dialog.pack();
            profile_dialog.setSize(200, 300);
            if(profile_source != null)
                profile_source.setSendProfile(false);
            wave.setSendProfile(true);
            profile_source = wave;
            profile_dialog.show();
            wave.sendProfileEvent();
        }
    }
   
   
  public void PrintAll(Graphics g, int st_x, int st_y, int height, int width)
  {
	  
	if(title != null && title.length() != 0)
	{
	     FontMetrics fm;
	     int s_width;

	     if(font == null)
	     {
	        font = g.getFont();
	        font = new Font(font.getName(), font.getStyle(), 18);
	        g.setFont(font);
	     }
	     else
	     {
	        font = new Font(font.getName(), font.getStyle(), 18);
	        g.setFont(font);
	     }
	     
	     fm = g.getFontMetrics();
	     
	     st_y += fm.getHeight() + 6;
	     height -= st_y + 30;
         
         s_width = fm.stringWidth(title);

         g.drawString(title, st_x + (width - s_width)/2, st_y - 2);
	}
	
	super.PrintAll(g, st_x, st_y, height, width);  
 }         
  
  public void StartUpdate()
  {
      if(!updateThread.isAlive())
      {
        updateThread = new UpdW();
        updateThread.start();
      }
      updateThread.StartUpdate();
  }

  public void StartPrint(PrinterJob prnJob, PageFormat pf)
  {
    try
    {
      initMdsWaveInterface();
      UpdateAllWave();
      PrintAllWaves(prnJob, pf);
    } catch (InterruptedException e){}
      catch (PrinterException e){}
      catch (Exception e){}
  }
  
  public int[] getMainShots()
  {
     try
     {
        if(main_shot_changed)
            evaluateMainShot(main_shot_str);
     } catch (IOException e){}
     return main_shots;
  }
  
  public String getMainShotStr()
  {
    try
    {
        if(main_shot_changed)
            evaluateMainShot(main_shot_str);
    } catch (IOException e){}
    return main_shot_str;
  }

  public void setMainShotStr(String main_shot_str)
  {
    if(main_shot_str == null || !main_shot_str.equals(this.main_shot_str))
       main_shot_changed = true;
    this.main_shot_str = main_shot_str;
  }

  public String getMainShotError(boolean brief)
  {
    //if(brief)
    //    return main_shot_error.substring(0, main_shot_error.indexOf("\n"));
    //else
        return main_shot_error;
  }

  public void evaluateMainShot(String in_shots) throws IOException
  {
        String error = null;
	    int int_data[] = null;

        main_shot_error = null;
        
	    if(in_shots == null || in_shots.trim().length() == 0)
	    {
	        main_shot_error = "Main shot value Undefine";
            main_shots = null;
            main_shot_str = null;
	        return;
        }
        
//        if(main_shot_str != null && main_shot_str.equals(in_shots) && main_shots != null)
//            return null;

        main_shots = null;
        main_shot_str = null;
	
	    dp.Update(null, 0);
	    int_data = dp.GetIntArray(in_shots);
	    if( int_data == null || int_data.length == 0 || int_data.length > MdsWaveInterface.MAX_NUM_SHOT)
	    {
	        if(int_data != null && int_data.length > MdsWaveInterface.MAX_NUM_SHOT)
                error = "Too many shots. Max shot list elements " + MdsWaveInterface.MAX_NUM_SHOT +"\n";
	        else {
		        if(dp.ErrorString() != null)	    
		            main_shot_error = dp.ErrorString();
		        else
		            main_shot_error = "Shot syntax error\n";
	        }
	    }
	    
	    if(main_shot_error == null)
	    {
	        main_shot_changed = false;
                main_shots = int_data;
	    }
        main_shot_str = in_shots.trim();
  }

  
  public void AbortUpdate()
  {
        abort = true;
  }


  public void RemoveAllEvents(NetworkEventListener l)  throws IOException
  {
     jScopeMultiWave w;
     MdsWaveInterface wi; 
     
     if(dp == null)
        return;
    
     if(event != null && event.length() != 0)
		 dp.removeNetworkEventListener(l, event);
     
     if(print_event != null && print_event.length() != 0)
		 dp.removeNetworkEventListener(l, print_event);      
        
	 for(int i = 0, k = 0; i < 4; i++)
	 {
		for(int j = 0; j < getComponentsInColumn(i); j++, k++) 
	    {
	        w = (jScopeMultiWave)getGridComponent(k);
	        w.RemoveEvent();
        }
     }
  }
   
  public synchronized void AddAllEvents(NetworkEventListener l)  throws IOException
  {
     jScopeMultiWave w;
     MdsWaveInterface wi; 

     if(dp == null)
        return;

     if(event != null && event.length() != 0)
		 dp.addNetworkEventListener(l, event);
     
     if(print_event != null && print_event.length() != 0)
		 dp.addNetworkEventListener(l, print_event);      
    
	 for(int i = 0, k = 0; i < 4; i++)
	 {
		for(int j = 0; j < getComponentsInColumn(i); j++, k++) 
		{
	        w = (jScopeMultiWave)getGridComponent(k);
	        w.AddEvent();
        }
     }
  }


    public void InvalidateDefaults()
    {           
        jScopeMultiWave w;
	    for(int i = 0, k = 0; i < rows.length; i++)
	    {
		    for(int j = 0, n_error; j < rows[i]; j++, k++) 
		    {
	            w = (jScopeMultiWave)getGridComponent(k);
	            if(w != null)
		            w.wi.default_is_update = false;
		    }
	    }
    }
   
    public boolean GetFastNetworkState()
    {
        return fast_network_access;
    }
    
    public void SetFastNetworkState(boolean state)
    {
        jScopeMultiWave w;
    	fast_network_access = state;
	    for(int i = 0; i < getComponentNumber(); i++)
        {
	        w = (jScopeMultiWave)getGridComponent(i);
	        if(w != null && w.wi != null ) {
		       w.wi.full_flag = !state;
		       w.wi.setModified(true);
		       //Refresh(w, "Refresh");
		    }
		}
    }    

    public void EraseAllWave()
    {
        jScopeMultiWave w;
	    for(int i = 0; i < getComponentNumber(); i++)
        {
	        w = (jScopeMultiWave)getGridComponent(i);
	        if(w != null)
		       w.Erase();
		}
    }
        
    public synchronized void UpdateAllWave() throws Exception
    {
        jScopeMultiWave w;
        WaveContainerEvent wce;
        
        int curr_mode = GetMode();
	    //SetMode(Waveform.MODE_WAIT);
    	setCursor(new Cursor(Cursor.WAIT_CURSOR));
    	
        try
        {

            if(mds_wi == null)
                abort = true;
            else
	            abort = false;
	    	
	    	if(def_vals != null && def_vals.public_variables != null) 
	    	    dp.SetEnvironment(def_vals.public_variables);
	    	 

	        for(int i = 0, k = 0; i < 4 && !abort; i++)
	        {
	            for(int j = 0; j < rows[i]; j++, k++) 
		        {
		            if(mds_wi[k] != null)
		                mds_wi[k].Update();
		            //w = (jScopeMultiWave)getGridComponent(k);
		            //((MdsWaveInterface)w.wi).Update();
		        }
	        }

	        //    Initialize wave evaluation
	        for(int i = 0, k = 0; i < 4 && !abort; i++)
	        {
		        for(int j = 0; j < rows[i] && !abort; j++, k++) 
		        {
		        // w = (jScopeMultiWave)getGridComponent(k);
		        // if( w.wi!= null && w.wi.error == null)
		            if(mds_wi[k] != null && mds_wi[k].error == null)
		            {
			            wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, 
							        "Start Evaluate column " + (i + 1) + " row " + (j + 1));
                        dispatchWaveContainerEvent(wce);
			            //w.wi.StartEvaluate();
			            mds_wi[k].StartEvaluate();
		            }
		        }
	        }

        
	        if(main_shots != null)
	        {
		        for(int l = 0; l < main_shots.length && !abort; l++)
		        {
		            for(int i = 0, k = 0; i < 4 && !abort; i++)
		            {
			            for(int j = 0; j < rows[i] && !abort; j++, k++)
			            {
    //		                w = (jScopeMultiWave)getGridComponent(k);
    //			            MdsWaveInterface wi = (MdsWaveInterface)w.wi;
    //			            if(wi != null && wi.error == null && wi.num_waves != 0 && wi.UseDefaultShot())
			                if(mds_wi[k] != null && mds_wi[k].error == null && mds_wi[k].num_waves != 0 && mds_wi[k].UseDefaultShot())
			                {
					            wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, 
								                "Update signal column " + (i + 1) + " row " + (j + 1) + 
								                " main shot " + main_shots[l]);
					            dispatchWaveContainerEvent(wce);
    //				            wi.EvaluateShot(main_shots[l]);			
				                mds_wi[k].EvaluateShot(main_shots[l]);			
				            }
			            }
			        } 
		        }
	        }
    	    
	        // Evaluate evaluate other shot	
        
	        for(int i = 0, k = 0; i < 4 && !abort; i++)
	        {
		        for(int j = 0; j < rows[i] && !abort; j++, k++) 
		        {
		    //      w = (jScopeMultiWave)getGridComponent(k);
            //      if(w.wi != null && w.wi.error == null && w.wi.num_waves != 0)
			        if(mds_wi[k] != null && mds_wi[k].error == null && mds_wi[k].num_waves != 0)
                    {
                        wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, 
                                "Evaluate wave column " + (i + 1) + " row " + (j + 1));
                        dispatchWaveContainerEvent(wce);
			        //    w.wi.EvaluateOthers();
			            mds_wi[k].EvaluateOthers();
    			        
		            }		    
		        }
	        }	    

            SwingUtilities.invokeLater(new Runnable() 
            {
                public void run()
                {
                    jScopeMultiWave wx;
	                for(int i = 0, k = 0; i < 4 && !abort; i++)
	                {
		                for(int j = 0; j < rows[i] && !abort; j++, k++) 
		                {
	    	                wx = (jScopeMultiWave)getGridComponent(k);
	    	                if(wx.wi != null)
			                    wx.Update(wx.wi);
		                }
                    }
                }
             });
            
             mds_wi = null;
            
	 //       System.gc();
	//        SetMode(curr_mode);

	        setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        
        } catch (Exception e) 
        {
   //       System.gc();
  //        SetMode(curr_mode);
  	        setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
            throw(new Exception(""+e));
        }
    }

    private String AddRemoveEvent(NetworkEventListener l, String curr_event, String event)  throws IOException
    {
        if(curr_event != null && curr_event.length() != 0)
        {
            if(event == null || event.length() == 0)
            {
                dp.removeNetworkEventListener(l, curr_event);
                return null;
            } 
            else 
            {
                if(!curr_event.equals(event))
                {
                    dp.removeNetworkEventListener(l, curr_event);
                    dp.addNetworkEventListener(l, event);
                    return event;
                }
            }
         } 
         else 
         {
            if(event != null && event.length() != 0) 
                dp.addNetworkEventListener(l, event);
                return event;
         }
         return null;
    }

    public void SetEvent(NetworkEventListener l, String event) throws IOException
    {
        this.event = AddRemoveEvent(l, this.event, event);
    }
    
    public void SetPrintEvent(NetworkEventListener l, String print_event) throws IOException
    {
        this.print_event = AddRemoveEvent(l, this.print_event, print_event);
    }


    public String FromFile(ReaderConfig in, String prompt) throws IOException
    {        
	    String str;
	    String error = null;
	    int read_rows[] = {0,0,0,0};

     
            in.reset();
	    while((str = in.readLine()) != null) 
	    {
	        //if(str.trim().length() != 0 && str.indexOf("Scope") != 0) 
	        //{
		    //    return 1;
	        //}
	    	    
	        if(str.indexOf("Scope.columns:") == 0) {
		        int len = "Scope.columns: ".length();
		        columns = new Integer(str.substring(len, str.length())).intValue();
		        pw = new float[MAX_COLUMN];
		        continue;
	        }
	    
	        if(str.indexOf("Scope.title:") == 0) {
		        title = str.substring("Scope.title: ".length(), str.length());
		        continue;
	        }

	        if(str.indexOf("Scope.update_event:") == 0) {
		        event = str.substring("Scope.update_event: ".length(), str.length());
		        continue;
	        }

	        if(str.indexOf("Scope.print_event:") == 0) {
		        print_event = str.substring("Scope.print_event: ".length(), str.length());
		        continue;
	        }

	        if(str.indexOf("Scope.fast_network_access:") != -1)
	        {
		        fast_network_access =  new Boolean(str.substring("Scope.fast_network_access: ".length(), 
										str.length())).booleanValue();
		        continue;		
	        }
	    
	        if(str.indexOf("Scope.data_server_address:") != -1)
	        {
		        server_name = str.substring("Scope.data_server_address: ".length(), str.length());
                //if(main_scope.applet_type != jScope.PURE_APPLET)
                //    main_scope.server_diag.addServerIp(data_server_address);
		        continue;	
	        }
					
	        if(str.indexOf("Scope.rows_in_column_") != -1)
	        {
		        int len;
		        int c = new Integer(str.substring("Scope.rows_in_column_".length(), len = str.indexOf(":"))).intValue() - 1;
		        int r = new Integer(str.substring(len + 2, str.length())).intValue();

		        if(c > MAX_COLUMN)
		        {
		            error = "Column number exceed max number of column "+ MAX_COLUMN;
		            break;
		        }
		        read_rows[c] = r;
		        continue;		
	        }
	    
	        if(str.indexOf("Scope.vpane_") != -1)
	        {
		        int len;
		        int c = new Integer(str.substring("Scope.vpane_".length(), len = str.indexOf(":"))).intValue() - 1;
		        int w = new Integer(str.substring(len + 2, str.length())).intValue();
		        if(c > columns)
		        {
		            error = "Column number exceds number of column defined in file";
		            break;
		        }
		        pw[c] = (float)w;
		        continue;		
	        }		

	        if(str.indexOf("Scope.reversed:") != -1)
                {
                   reversed = new Boolean(str.substring("Scope.reversed: ".length(), str.length())).booleanValue();
                }
	    
	        if(str.indexOf("Scope.global_1_1") == 0)
	        {
		        int len = str.indexOf(":") + 2;

		        if(str.indexOf(".xmax:") != -1)
		        {
		            def_vals.xmax = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".xmin:") != -1)
		        {
		            def_vals.xmin = str.substring(len, str.length());
		            continue;		
		        }
				
		        if(str.indexOf(".x_label:") != -1)
		        {
		            def_vals.xlabel = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".ymax:") != -1)
		        {
		            def_vals.ymax = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".ymin:") != -1)
		        {
		            def_vals.ymin = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".y_label:") != -1)
		        {
		            def_vals.ylabel = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".experiment:") != -1)
		        {
		            def_vals.experiment_str = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".title:") != -1)
		        {
		            def_vals.title_str = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".shot:") != -1)
		        {
		            def_vals.shot_str = str.substring(len, str.length());
		            if(def_vals.shot_str.indexOf("_shots") != -1) 
			        def_vals.shot_str  =  def_vals.shot_str.substring(def_vals.shot_str.indexOf("[")+1, def_vals.shot_str.indexOf("]")); 
//aa		    def_vals.shots = main_scope.evaluateShot(def_vals.shot_str);
                    def_vals.is_evaluated = false;
                    continue;		
		        }		
		        if(str.indexOf(".event:") != -1)
		        {
		            def_vals.upd_event_str = str.substring(len, str.length());
		            continue;		
		        }
		        if(str.indexOf(".default_node:") != -1)
		        {
		            def_vals.def_node_str = str.substring(len, str.length());
		            continue;		
		        }   
            }	    	           
	    }
	    
	    ResetDrawPanel(read_rows);
	    jScopeMultiWave w;
	    
            for(int c = 0, k = 0; c < 4 ; c++)
		{
		    for(int r = 0; r < read_rows[c]; r++)
		    {
		        w = (jScopeMultiWave)getGridComponent(k);
			w.wi.FromFile(in, "Scope.plot_"+(r+1)+"_"+(c+1));
//                setColor(wave_panel.waves[k].wi);
			k++;
			    //setup.SetStatusLabel("Load wave configuration column "+(c+1)+" row "+(r+1));	    
		    }
		}

	
	    //Evaluate real number of columns
	    int r_columns =  0;
	    for(int i = 0; i < 4; i++)
	        if(read_rows[i] != 0)
		        r_columns = i + 1;

	    if(pw == null)
	    {
	        pw = new float[MAX_COLUMN];
	        columns = 0;
        }
        
	    //Silent file configuration correction
	    //possible define same warning information	    	    

	    if(columns != r_columns)
	    {			
		    columns = r_columns;
		    pw = new float[MAX_COLUMN];
		    for(int i = 0; i < columns; i++)
		        pw[i] = (float) 1./columns;
	    } else {			    
		        if(columns == 4)
		            pw[3] = Math.abs((float)((1000 - pw[2])/1000.));
		        if(columns >= 3)
		            pw[2] = Math.abs((float)(((pw[2] != 0) ? (pw[2] - pw[1]): 1000 - pw[1])/1000.));
		        if(columns >= 2)
		            pw[1] = Math.abs((float)(((pw[1] != 0) ? (pw[1] - pw[0]): 1000  - pw[0])/1000.));
		        if(columns >= 1)
		            pw[0] = Math.abs((float)(((pw[0] == 0) ? 1000 : pw[0])/1000.));
	    }
	
	    if(error == null)
	        UpdateHeight();
	    
	    return error;
    }


    public void UpdateHeight()
    {
	    float height = 0;
//        Dimension d = waves[0].getMinimumSize();
        jScopeMultiWave w;
	
	    ph = new float[getComponentNumber()];
    
	    for(int j = 0, k = 0; j < columns; j++)
	    {
	        height = 0;
    	    for(int i=0; i < rows[j]; i++) {
		        w = (jScopeMultiWave)getGridComponent(k+i);
	            height += w.wi.height;
            }
	        for(int i = 0; i < rows[j]; i++, k++)
	        {
		        w = (jScopeMultiWave)getGridComponent(k);
		        //if( w.wi.height < d.height)//RowColumnLayout.MIN_SIZE_H)
		        if(height == 0)
		        {
		            k -= i;
		            for(i = 0; i < rows[j]; i++)
			            ph[k++] = (float)1./rows[j];			
		            break;
		        }
	            ph[k] = (float)(w.wi.height/height);
	        }
	    }
	    invalidate();
  }

  private boolean IsIpAddress(String addr)
  {
    if(addr.trim().indexOf(".") != -1 && addr.trim().indexOf(" ") == -1)
        return true;
    else
        return false;
  }

    public String SetDataServer(String new_data_server, NetworkEventListener l)
    {
	    boolean change = false;  
	    //DataProvider old_dp = dp;
	    DataProvider new_dp = null;
	    String error = null;
	    
	    if(new_data_server == null || new_data_server.trim().length() == 0)
	        return "Defined null or empty data server name";
	    
        if(new_data_server.equals("Local") && supports_local)
        {
	        try {
		        new_dp = new LocalProvider();
		        fast_network_access = false;
		        supports_fast_network = false;
		        supports_local = true;
		        change = true;	
	        } catch (Throwable ex) {
		        supports_local = false;
		        error = "Local data access is not supported on this platform";
		        change = false;
	        }
	    } 
	    
//Gabriele 26/10/99
        if(new_data_server.equals("Jet data"))
        {
            new_dp = new JetDataProvider();
            Container c = this;
            while(c != null && !(c instanceof JFrame))
                c = c.getParent();
            ((JetDataProvider)new_dp).InquireCredentials((JFrame)c);//(JFrame)this.getParent().getParent());
            int option = ((JetDataProvider)new_dp).GetLoginStatus();
            switch(option)
            {
                case JetDataProvider.LOGIN_OK :
		            fast_network_access = false;
		            supports_fast_network = false;
                    change = true;
                break;
                case JetDataProvider.LOGIN_ERROR :
                    if(dp == null)
                        SetServerName("Not connected");
                    return "Invallid Login";
                case JetDataProvider.LOGIN_CANCEL :
                    if(dp == null)
                        SetServerName("Not connected");
                     return null;         
            }
        } 
    
        if(new_data_server.indexOf("Ftu data") != -1)
        {
            try
            {
	            String ip = new_data_server.substring(new_data_server.indexOf(":")+1, new_data_server.length());
                new_dp = new FtuProvider(ip);
                fast_network_access = false;
		        supports_fast_network = true;
                change = true;
            } 
            catch (IOException ex) 
            {
		        error = "Ftu data server error :" + ex.getMessage();
		        change = false;
            }
        } 

        if(new_data_server.equals("Demo server"))
        {
            new_dp = new DemoProvider();
 		    fast_network_access = false;
		    supports_fast_network = false;
            change = true;
        }

        if(!change && IsIpAddress(new_data_server))
        {
	        new_dp = new NetworkProvider(new_data_server);
            fast_network_access = false;
		    supports_fast_network = true;
            change = true;
        }

	    if(change) 
	    {
	        try
	        {
	            RemoveAllEvents(l);
	        
	            if(dp != null) {
                    dp.removeNetworkTransferListener((NetworkTransferListener)l);
                    dp.disconnect();
                }
                
	            dp = new_dp;
                if(dp != null)
                    dp.addNetworkTransferListener((NetworkTransferListener)l);
                    
                SetServerName(new_data_server);
                ChangeDataProvider(dp);
                AddAllEvents(l);
            } 
            catch (IOException e) 
            {
                SetServerName("Disconnected < "+new_data_server+" >");
                dp = null;
                error = e.getMessage();
            }
	    } else
            error = "Data server "+ new_data_server +" not defined\n";        
	    
	    	
	    return error;
    }
    

    public String AddSignal(String expr)
    {
        MdsWaveInterface old_wi = null;
        jScopeMultiWave sel_wave = (jScopeMultiWave)GetSelectPanel();
        String error = null;
            

        if(sel_wave.wi == null)
            sel_wave.wi = new MdsWaveInterface(sel_wave, dp, def_vals);
        else {
            old_wi = new MdsWaveInterface((MdsWaveInterface)sel_wave.wi);
            old_wi.wave = sel_wave;
        }
        
        if(sel_wave.wi.AddSignal(expr)) 
        {
            add_sig = true;
            //error = EvaluateWave(sel_wave, true);
            error = Refresh(sel_wave, "Add signal to");
            if(error != null)
            {
              sel_wave.wi = old_wi;
             // EvaluateWave(sel_wave, false);
             // Refresh(sel_wave, "");
            } //else
              //sel_wave.SetErrorTitle();
            update();
            add_sig = false;
	    }
	    return error;
    }

    public void ToFile(PrintWriter out, String prompt) throws IOException
    {
	    jScopeMultiWave w;
        MdsWaveInterface wi;

	    WaveInterface.WriteLine(out, prompt + "title: ", title);		    
	    WaveInterface.WriteLine(out, prompt + "data_server_address: ",    server_name);		    
	    WaveInterface.WriteLine(out, prompt + "fast_network_access: ", ""+fast_network_access);		    
	    WaveInterface.WriteLine(out, prompt + "update_event: ", event);		    
	    WaveInterface.WriteLine(out, prompt + "print_event: ", print_event);
        WaveInterface.WriteLine(out, prompt + "reversed: "     , ""+reversed);

	    out.println();
	     
	    WaveInterface.WriteLine(out,prompt + "global_1_1.experiment: "   , def_vals.experiment_str);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.event: "        , def_vals.upd_event_str);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.default_node: " , def_vals.def_node_str);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.shot: "         , def_vals.shot_str);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.title: "        , def_vals.title_str);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.xmax: "         , def_vals.xmax);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.xmin: "         , def_vals.xmin);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.x_label: "       , def_vals.xlabel);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.ymax: "          , def_vals.ymax);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.ymin: "          , def_vals.ymin);
	    WaveInterface.WriteLine(out,prompt + "global_1_1.y_label: "       , def_vals.ylabel);
	    
	    out.println();
	    	    
	    out.println("Scope.columns: " + getColumns());
	
	    for(int i = 0, c = 1, k = 0; i < getColumns(); i++,c++)
	    {
		    WaveInterface.WriteLine(out, prompt + "rows_in_column_" + c + ": " , ""+getComponentsInColumn(i));
    		for(int j = 0, r = 1; j < getComponentsInColumn(i); j++, r++)
		    {	
		        w = (jScopeMultiWave)getGridComponent(k);
			    wi = (MdsWaveInterface)w.wi;
		        
		        out.println("\n");

		        WaveInterface.WriteLine(out, prompt + "plot_" + r + "_" + c + ".height: "          , ""+w.getSize().height );
		        WaveInterface.WriteLine(out, prompt + "plot_" + r + "_" + c + ".grid_mode: "       , ""+w.grid_mode);
                if(wi != null)
		            wi.ToFile(out, prompt + "plot_" + r + "_" + c + ".");
		        k++;
		    }
	     }

	    out.println();
	    
	     for(int i = 1, k = 0, pos = 0; i < getColumns(); i++)
	     {
		    w = (jScopeMultiWave)getGridComponent(k);
			wi = (MdsWaveInterface)w.wi;
		    pos += (int)(((float)w.getSize().width/ getSize().width) * 1000.);
		    WaveInterface.WriteLine(out, prompt + "vpane_" + i + ": " , ""+pos);
		    k += getComponentsInColumn(i);
	     } 	    
    }


    public synchronized String Refresh(jScopeMultiWave w, String label)
    {        
        Point p = null;
        int mode;        

        if(add_sig)
            p = getSplitPosition();
        if(p == null)
            p = getComponentPosition(w);
	    
        WaveContainerEvent wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, label+" wave row " + p.x + " column " + p.y);
        jScopeWaveContainer.this.dispatchWaveContainerEvent(wce);
	        
	    String error = w.Refresh(add_sig, brief_error);
	    
        wce = new WaveContainerEvent(this, WaveContainerEvent.START_UPDATE, "Wave row " + p.x + " column "+ p.y + " is updated");
        jScopeWaveContainer.this.dispatchWaveContainerEvent(wce);

        if(add_sig)
            resetSplitPosition();

	    
	    return error;
    }    
    
    public void SaveAsText(jScopeMultiWave w)
    {
        if(w == null || w.wi == null || w.wi.signals.length == 0) return;
                
        JFileChooser file_diag = new JFileChooser();
        
        int returnVal = file_diag.showSaveDialog(this);
        
        if (returnVal == JFileChooser.APPROVE_OPTION) 
        {    	  
	        File file = file_diag.getSelectedFile();
	        String txtsig_file = file.getAbsolutePath();
	        if(txtsig_file != null)
	        {
                String s = "", s1="", s2="";
                boolean more_point, new_line;
                StringBuffer space = new StringBuffer();
                
                try {
	                BufferedWriter out = new BufferedWriter(new FileWriter(txtsig_file));
	                for(int l = 0 ; l < 3; l++)
	                {
	                    for(int i = 0; i < w.wi.signals.length; i++)
                        {
                            switch(l)
                            {
                            case 0 : s = "x : "    + ((w.wi.in_x != null && w.wi.in_x.length > 0) ? w.wi.in_x[i]: "None"); break;
                            case 1 : s = "y : "    + ((w.wi.in_y != null && w.wi.in_y.length > 0) ? w.wi.in_y[i]: "None"); break;
                            case 2 : s = "Shot : " + ((w.wi.shots != null && w.wi.shots.length > 0) ? ""+w.wi.shots[i] : "None"); break;
                            }
                            out.write(s, 0, (s.length() < 34) ? s.length() : 34);
                            space.setLength(0);
                            for(int u = 0; u < 35 - s.length(); u++)
                                space.append(' ');
                            out.write(space.toString());
                        }
	                    out.newLine();
                    }
    	            
                    more_point = true;
                    int j = 0;
                    double xmax = w.GetWaveformMetrics().XMax();
                    double xmin = w.GetWaveformMetrics().XMin();
                    int start_idx[] = new int[w.wi.signals.length];
                    
	                for(int i = 0; i < w.wi.signals.length; i++)
	                    start_idx[i] = 0;
    	                
                    while(more_point)
                    {
                        more_point = false;
	                    for(int i = 0; i < w.wi.signals.length; i++)
                        {
                            s1 = "";
                            s2 = "";
                            if(w.wi.signals[i] != null && w.wi.signals[i].x != null)
                            {
                                for(j = start_idx[i]; j < w.wi.signals[i].x.length; j++) 
                                {
                                    if(w.wi.signals[i].x[j] > xmin &&
                                    w.wi.signals[i].x[j] < xmax)
                                    {   
                                        more_point = true;
                                        s1 = ""+w.wi.signals[i].x[j];
                                        s2 = ""+w.wi.signals[i].y[j];
                                        start_idx[i] = j+1;
                                        break;
                                    } 
                                }
                            }
                            out.write(s1);
                            space.setLength(0);
                            for(int u = 0; u < 15 - s1.length(); u++)
                                space.append(' ');
                            space.append(' ');
                            out.write(space.toString());
                            out.write(" ");
                            out.write(s2);
                            space.setLength(0);
                            for(int u = 0; u < 18 - s2.length(); u++)
                                space.append(' ');
                            out.write(space.toString());                            
                        }
	                    out.newLine();
                    }                
	                out.close();

	            } catch (IOException e) {
	                System.out.println(e);
	            }
	        }
	    }
	    file_diag = null;
    }
}
