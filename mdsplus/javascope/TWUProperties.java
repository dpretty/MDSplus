/* ------------------------------------------------------------------------ */
/*
 * TWUProperties: Handling the Web-Umbrella Signal Properties.
 *
 * Copyright (c) 2002, J.G.krom, Forschungszentrum J�lich GmbH.
 * All rights reserved.
 * 
 * This class handles the properties 'file' as used in the TEC Web-Umbrella.
 *
 * NOTE:
 * The TWU* classes (with TWU in three capitals) are written so that they 
 * can also be used in other java based applications and not only by jScope.  
 * Please keep the code of these separate of jScope specific elements.
 *
 * This class does not throw any exceptions; this should be left so.  This is
 * because it is also used in contexts where the handling of such exceptions 
 * is somewhat troublesome. (E.g. JVMs running implicitly by matlab.)
 *
 * Author: Jon Krom, Forschungszentrum J�lich, Institut f�r Plasmaphysik.
 *
 * $Id: TWUProperties.java,v 1.21 2003/03/14 15:43:08 jgk Exp $
 *
 * ------------------------------------------------------------------------ 
 * $Log: TWUProperties.java,v $
 * Revision 1.21  2003/03/14 15:43:08  jgk
 * Some unimportant changes to the comment texts, in effect
 * going back to the situation of revision 1.17.
 *
 * Revision 1.20  2003/02/28 09:50:59  manduchi
 * New jScope version (7.2.2) supporting ssh tunneling
 *
 * Revision 1.19  2003/01/09 16:49:41  jgk
 * Another trivial change to test the CVS notification system.
 *
 * Revision 1.18  2003/01/09 14:35:58  jgk
 * Some trivial changes to test the CVS notification system.
 *
 * Revision 1.17  2002/11/28 11:56:51  jgk
 * Some unimportant changes to the comment texts, in effect
 * going back to the situation of revision 1.13.
 *
 * Revision 1.16  2002/11/12 17:18:29  manduchi
 * Added support for waveform editing
 *
 * Revision 1.13  2002/09/20 12:37:08  jgk
 * Factored-out the (nested) class TwuFetchOptions in to its own source file.
 * Clarified a minor naming convention: Classes with names starting with TWU
 * are written such that they are not strongly bound to jScope, but can also
 * be used by other java based TEC Web-Umbrella clients.
 *
 * Revision 1.12  2002/09/04 14:11:25  manduchi
 * Fixed porting to jdk1.4
 *
 * Revision 1.8  2002/05/27 13:49:51  jgk
 * Suggestion from Marco: NaN for min/max when the're no proper values.
 *
 * Revision 1.7  2002/05/24 08:55:19  jgk
 * A bit more robust when faced with HTTP servers that do not provide
 * a mime_type.   Assume it to be text/html in these cases (like most
 * browsers seem to do).
 *
 * Revision 1.6  2002/05/07 11:24:31  jgk
 * Improvement to the Units() method.
 *
 * Revision 1.5  2002/05/06 16:02:05  jgk
 * Suggested by  Marco van de Giessen <A.P.M.vandeGiessen@phys.uu.nl>:
 * - equalsIgnoreCase() is more generally useful than equals().
 * - A faked properties page can sometimes be useful.
 *
 * Revision 1.4  2002/04/26 11:42:38  jgk
 * Changed End-Of-Line marker from Windows- to CVS standard.
 *
 * Revision 1.3  2002/04/24 09:06:17  manduchi
 * Added Asdex Data Provider
 *
 * Derived from (matlab-) WUProperties.java Revision 1.6 2002/02/26 hrk/jgk
 */
/* ------------------------------------------------------------------------ */

import java.net.*;
import java.io.*;
import java.util.Properties;

public class TWUProperties
{
    private   URL        signalURL   = null;
    protected Properties signalProps = new Properties();
    private   boolean    textRead    = false;
    
    /* -------------------------------------------------------------------- */
    // Constructors

    public
    TWUProperties(String SigURL)
    {
        this (SigURL, null);
    }

    public
    TWUProperties(String SigURL, String user_agent)
    {
        final String
            actual_user_agent = (user_agent!=null) ?  user_agent
            : "TWUProperties.java for jScope ($Revision: 1.21 $)";
        signalProps = new Properties();

        if (SigURL==null)
          return;
                
        final String fullURL = CompleteURL(SigURL);

        try
        {
            signalURL   = new URL(fullURL);

            URLConnection con = signalURL.openConnection();
            con.setRequestProperty("User-Agent",actual_user_agent);
            String mime_type = con.getContentType();

            // Assume (like browsers) that missing mime-type indicates text/html.
            if(mime_type==null || mime_type.indexOf("text") >=0)
            {
                signalProps.load(con.getInputStream());
                textRead = true;
            }
        }
        catch (Exception e) 
        {
            signalURL = null;
            /* No need for further action; textRead will be left false. */
        }
    }

    // Allow, like many browsers do, URLs without protocol name (the
    // 'http:' bit).  Assume in these cases that we want 'http:'
    // This allows. (at least in principle) other protocols.

    private String CompleteURL(String in)
    {
        if(in.indexOf("://") >= 0)
          return in;
        return "http:" + in;
    }


    /* -------------------------------------------------------------------- */
    // Accessors:

    public boolean
    valid()
    {
        // Should handle this pseudo-numerically
        String version = signalProps.getProperty("TWU.properties.version");
        return textRead 
            && version!=null 
            && (version.equals("0.7") || version.equals("0.8") );
    }

    public int 
    Dimensions()
    {
        String  dimstr = signalProps.getProperty("Dimensions");
        Integer dim = Integer.valueOf(dimstr==null? "0" : dimstr);
        return  dim.intValue();
    }
    
    public int 
    Dim0Length()
    {
        String  ln0str = signalProps.getProperty("Length.dimension.0");
        Integer d0l = Integer.valueOf(ln0str==null? "0" : ln0str);
        return  d0l.intValue();
    }
    
    public int 
    Dim1Length()
    {
        String  ln1str = signalProps.getProperty("Length.dimension.1");
        Integer d1l = Integer.valueOf(ln1str==null? "0" : ln1str);
        return  d1l.intValue();
    }
    
    public int 
    LengthTotal()
    {
        String  ltstr = signalProps.getProperty("Length.total");
        Integer lt = Integer.valueOf(ltstr==null? "0" : ltstr);
        return  lt.intValue();
    }
    
    public String
    Title()
    {
        String title = signalProps.getProperty("Title");
        if(title == null)
          title = signalProps.getProperty("SignalName");
        return title;
    }

    public String
    FQSignalName()
    {
        return signalProps.getProperty("SignalURL");
    }

    public String
    SignalName()
    {
        return signalProps.getProperty("FullSignalName");
    }

    public String
    Units()
    {
        String unitstr =  signalProps.getProperty("Unit");
        return ((unitstr == null || unitstr.equalsIgnoreCase("None")) ? "" : unitstr) ;
    }

    public String
    FQBulkName()
    {
        return signalProps.getProperty("Bulkfile.URL");
    }
    
    public boolean
    hasAbscissa()
    {
        return hasAbscissa0();
    }

    public boolean
    hasAbscissa0()
    {
        String abscissa = signalProps.getProperty("Abscissa.URL.0");
        return ((abscissa == null) ? false : ( ! abscissa.equalsIgnoreCase("None"))) ;
    }

    public boolean
    hasAbscissa1()
    {
        String abscissa = signalProps.getProperty("Abscissa.URL.1");
        return ((abscissa == null) ? false : ( ! abscissa.equalsIgnoreCase("None"))) ;
    }

    public boolean
    Incrementing()
    {
        String equidistant = signalProps.getProperty("Equidistant");
        return (equidistant!=null) && equidistant.equalsIgnoreCase("incrementing") ;
    }

    public boolean
    Decrementing()
    {
        String equidistant = signalProps.getProperty("Equidistant");
        return (equidistant!=null) && equidistant.equalsIgnoreCase("decrementing") ;
    }

    public boolean
    Equidistant()
    {
        return
            (Incrementing() || Decrementing()) 
            && 
            signalProps.getProperty("Signal.Minimum") !=null
            && 
            signalProps.getProperty("Signal.Maximum") !=null;
    }

    public double
    Minimum()
    {
        String mini = signalProps.getProperty("Signal.Minimum");
        Double min  = new Double(Double.NaN);
        if (mini != null)
          min = Double.valueOf(mini);
        return min.doubleValue();
    }

    public double
    Maximum()
    {
        String maxi = signalProps.getProperty("Signal.Maximum");
        Double max  = new Double(Double.NaN);
        if (maxi != null)
          max = Double.valueOf(maxi);
        return max.doubleValue();
    }
    
    public double
    averageStep()
    {
        double span = Maximum()-Minimum();
        int segments = LengthTotal()-1;
        if (Decrementing())
          segments *= -1;
        return (segments==0 ? Double.NaN : span/segments);
    }
    
    public String
    FQAbscissaName()
    {
        String abs = signalProps.getProperty("Abscissa.URL.0");
        return ((abs==null || abs.equalsIgnoreCase("None")) ? null : abs);
    }

    public String
    FQAbscissa0Name()
    {
        String abs = signalProps.getProperty("Abscissa.URL.0");
        return ((abs==null || abs.equalsIgnoreCase("None")) ? null : abs);
    }

    public String
    FQAbscissa1Name()
    {
        String abs = signalProps.getProperty("Abscissa.URL.1");
        return ((abs==null || abs.equalsIgnoreCase("None")) ? null : abs);
    }

    /* -------------------------------------------------------------------- */

    // A generic getproperties method.
    public String
    getProperty(String keyword)
    {
        return signalProps.getProperty(keyword);
    }

    public boolean
    equals(String other_url)
    {
        try
        {
            return CompleteURL(other_url).equals(signalURL.toString());
        }
        catch(Exception e)
        {
            return false;
        }   
    }
    
    public boolean
    notEquals(String other_url)
    {
        return ! equals(other_url);
    }
    
    /* -------------------------------------------------------------------- */
    
    public String 
    toString()
    {
        return signalURL==null ? "" : signalURL.toString();
    }
    
        
    public String 
    summary()
    {
        return
            "Valid                  : " + valid() + "\n" +
            "Title                  : " + Title() + "\n" +
            "Signal Name            : " + SignalName() + "\n" +
            "Full Signal Name       : " + FQSignalName() + "\n" +
            "Bulk File Name         : " + FQBulkName() + "\n" +
            "Dimensions             : " + Dimensions() + "\n" +
            "Total Length           : " + LengthTotal() + "\n" +
            "Equidistant            : " + Equidistant() + "\n" +
            "Signal.Minimum         : " + Minimum() + "\n" +
            "Signal.Maximum         : " + Maximum() + "\n" +
            "averageStep            : " + averageStep() + "\n" +
            "Dimension [0] length   : " + Dim0Length() + "\n" +
            "Abscissa [0] File Name : " + 
            (hasAbscissa0() ? FQAbscissa0Name() : "No abscissa_0 for this signal") + "\n" +
            "Dimension [1] length   : " + Dim1Length() + "\n" +
            "Abscissa [1] File Name : " +
            (hasAbscissa1() ? FQAbscissa1Name() : "No abscissa_1 for this signal") + "\n"; 
    }
    
    public static String 
    revision()
    {
        return "$Id: TWUProperties.java,v 1.21 2003/03/14 15:43:08 jgk Exp $";
    }

    public static void 
    main (String a[]) 
    {
        TWUProperties twup ;

        System.out.println();
        System.out.println(revision());

        System.out.println();
        System.out.println("Web-Umbrella data from a webserver.");

        twup = new TWUProperties(
//          "//localhost/textor/all/91100/vuv/spred-a/H-I_@_102,550"); // Existing
//          "//localhost/textor/all/91100/vuv/spred-a/H-I_@_102,551"); // Non-existing
//          ""); // Non-existing
            null); 
        
        System.out.println(twup);
        System.out.println(twup.summary());

        System.out.println();

        twup = new TWUProperties(
            "//localhost/textor/all/91100/vuv/spred-a/H-I_@_102,550"); // Existing
        
        System.out.println(twup);
        System.out.println(twup.summary());

        System.out.println();
    }

}


class FakeTWUProperties extends TWUProperties {
  public FakeTWUProperties (int len) {
    super (null) ;
    signalProps.put ("TWU.properties.version", "0.7");
    signalProps.put ("Dimensions", "1");
    signalProps.put ("Length.total",       String.valueOf(len) );
    signalProps.put ("Length.dimension.0", String.valueOf(len) );
    signalProps.put ("Equidistant", "Incrementing");
    signalProps.put ("Signal.Minimum", "0.0");

    signalProps.put ("Signal.Maximum", String.valueOf( (double)(len-1) ) );
  }
}

/* ------------------------------------------------------------------------ */
// End of $Id: TWUProperties.java,v 1.21 2003/03/14 15:43:08 jgk Exp $
/* ------------------------------------------------------------------------ */
