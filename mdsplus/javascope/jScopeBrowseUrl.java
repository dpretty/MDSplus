 import javax.swing.border.*; 
 import javax.swing.colorchooser.*; 
 import javax.swing.filechooser.*; 
 import javax.accessibility.*; 
 import javax.swing.event.*; 
 import javax.swing.text.html.*; 
 import javax.swing.*; 
  
 import java.awt.*; 
 import java.awt.event.*; 
 import java.beans.*; 
 import java.util.*; 
 import java.io.*; 
 import java.applet.*; 
 import java.net.*; 
 
 public class jScopeBrowseUrl extends JDialog
 {   
     JEditorPane html;
     Vector url_list = new Vector();
     JButton back;
     JButton forward;
     JButton home;
     int curr_url = 0;
     JPanel p; 
      
     public jScopeBrowseUrl(JFrame owner)
     { 
 	    super(owner);
        html = new JEditorPane();
        html.setEditable(false); 
        html.addHyperlinkListener(createHyperLinkListener()); 
 		 
 		JScrollPane scroller = new JScrollPane(); 
 		JViewport vp = scroller.getViewport(); 
 		vp.add(html); 
        getContentPane().add(scroller, BorderLayout.CENTER);
                
        p = new JPanel();
        back = new JButton("Back");
        back.setSelected(true);
        p.add(back);
        back.addActionListener(new ActionListener()
	        {
	            public void actionPerformed(ActionEvent e)
                {
                    if(curr_url - 1 >= 0)
                    {
                        try {
                            curr_url--;
 			                html.setPage((URL)url_list.elementAt(curr_url));
 			            } catch (IOException ioe) { 
 			                System.out.println("IOE: " + ioe); 
 			            } 
                    }
                }
	        }
	    );
                
        forward = new JButton("Forward");
        back.setSelected(true);
        p.add(forward);
        forward.addActionListener(new ActionListener()
	        {
	            public void actionPerformed(ActionEvent e)
                {
                    if(curr_url + 1 < url_list.size())
                    {
                        try {
                            curr_url++;
 			                html.setPage((URL)url_list.elementAt(curr_url));
 			            } catch (IOException ioe) { 
 			                System.out.println("IOE: " + ioe); 
 			            } 
  			        }
                }
	        }
	    );
  
        home = new JButton("Home");
        home.setSelected(true);
        p.add(home);
        home.addActionListener(new ActionListener()
	        {
	            public void actionPerformed(ActionEvent e)
                {
                    if(url_list.size() != 0)
                    {
                        try {
                            curr_url = 0;
 			                html.setPage((URL)url_list.elementAt(0));
 			            } catch (IOException ioe) { 
 			                System.out.println("IOE: " + ioe); 
 			            } 
  			        }
                }
	        }
	    );
     
                
        getContentPane().add(p, BorderLayout.NORTH);
        pack();
 	    setSize(680,700);
      
     }

     public void connectToBrowser(URL url) throws Exception
     {
        if(url != null)
        {
 		    url_list.addElement(url);
            if(url != null)
                html.setPage(url);
        }
     }

          
       
     public void connectToBrowser(String url_path) throws Exception
     {
 	    URL url = null;
 		url = new URL(url_path);
        connectToBrowser(url);
    }
  
     public HyperlinkListener createHyperLinkListener() { 
 	    return new HyperlinkListener() { 
 	        public void hyperlinkUpdate(HyperlinkEvent e)
 	        { 
 		        if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) 
 		        { 
 		            if (e instanceof HTMLFrameHyperlinkEvent) 
 		            { 
 			            ((HTMLDocument)html.getDocument()).processHTMLFrameHyperlinkEvent( 
 			            (HTMLFrameHyperlinkEvent)e); 
 		            } else { 
 			            try { 
 			                URL u = e.getURL();
 			                
 			                //To fix JVM 1.1 Bug
 			                if(u == null)
 			                {
 			                    HTMLDocument hdoc = (HTMLDocument)html.getDocument();
 			                    try {
 			                        
 			                        StringTokenizer st = new StringTokenizer(hdoc.getBase().toString(),"/");
 			                        int num_token = st.countTokens();
 			                        String base = st.nextToken()+"//";
 			                        for(int i = 0; i < num_token-2; i++)
 			                            base = base + st.nextToken()+"/";
 			                        if(jScope.is_debug)
 			                            System.out.println("JDK1.1 url = "+base+e.getDescription());  			                        
 			                        u = new URL(base+e.getDescription());
 			     		        } catch (MalformedURLException m) {
			                        u = null;
		                        }
                            }
 			                // end fix bug JVM 1.1
 			                
 			                html.setPage(u);
 			                
 			                int sz = url_list.size();
 			                for(int i = curr_url + 1; i < sz; i++)
 			                    url_list.removeElementAt(curr_url + 1);
 			                url_list.addElement(u);
 			                curr_url++;
			            } catch (IOException ioe) { 
 		                    JOptionPane.showMessageDialog(jScopeBrowseUrl.this, 
 		                                                  "IOE: " + ioe, 
		                                "alert", JOptionPane.ERROR_MESSAGE); 
 			            } 
 		            } 
 		        } 
 	        } 
 	    }; 
     }     
 } 
 
