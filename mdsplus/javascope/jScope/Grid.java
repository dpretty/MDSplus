package jScope;

/* $Id: Grid.java,v 1.6 2013/01/18 18:29:58 manduchi Exp $ */
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import java.io.*;
import java.text.*;
import java.util.*;

public class Grid
    implements Serializable
{
    WaveformMetrics wm;
    boolean reversed = false;
    int x_dim, y_dim;
    double x_values[], y_values[], x_step, y_step;
    int grid_step_x, grid_step_y;
    int mode;
    boolean int_ylabels, int_xlabels;
    Font font;
    int label_width, label_height, label_descent, num_x_steps, num_y_steps;
    String x_label, y_label, title, error;
    final static int IS_X = 0, IS_Y = 1;
    final static int IS_DOTTED = 0, IS_GRAY = 1, IS_NONE = 2, MAX_GRID = 10;
    final static String GRID_MODE[] = {"Dotted", "Gray", "None"};
    double xmax;

    boolean xAxisHMS = false;

    public Grid(double xmax, double ymax, double xmin, double ymin,
                boolean xlog, boolean ylog,
                int mode, String x_label, String y_label, String title,
                String error, int grid_step_x, int grid_step_y,
                boolean int_xlabels, boolean int_ylabels, boolean reversed)
    {
        this.reversed = reversed;
        this.mode = mode;
        this.x_label = x_label;
        this.y_label = y_label;
        this.title = title;
        this.error = error;
        this.grid_step_x = grid_step_x;
        this.grid_step_y = grid_step_y;
        this.int_xlabels = int_xlabels;
        this.int_ylabels = int_ylabels;
        font = null;
        x_values = new double[50];
        y_values = new double[50];
        this.xmax = xmax;

        x_dim = BuildGrid(x_values, IS_X, xmax, ymax, xmin, ymin, xlog, ylog);
        y_dim = BuildGrid(y_values, IS_Y, xmax, ymax, xmin, ymin, xlog, ylog);
    }

    void SetReversed(boolean reversed)
    {
        this.reversed = reversed;
    }

    
    static public double evalStep(double min, double max, int numStep)
    {
        double step = 0;
        
        double delta = Math.abs(max - min);
        int pow = (int) Math.log10(delta) - 1;
        
        double k = Math.pow(10, pow);
        
        int n1 = (int) ( delta / ( 2.  * k) );
        int n2 = (int) ( delta / ( 5.  * k)  );
        int n3 = (int) ( delta / ( 10. * k)  );
        
        return ( 2. * k );
        
    }
    
    private int BuildGridNew(double val[], int mode, 
                          double xmax, double ymax,
                          double xmin, double ymin, 
                          boolean xlog, boolean ylog)
    {


        if (ymax < ymin)
            ymax = ymin + 1E-10;
        if (xmax < xmin)
            xmax = xmin + 1E-10;
        
        double step, curr, curr_max, curr_min,
               xrange = xmax - xmin,
               yrange = ymax - ymin;
        
        boolean greater = false;
        boolean is_log;
        int grid_step;
        int count = 0, i, num_steps, rem_step = 1;
        Float f;
        
        if (xrange <= 0)
            xrange = (double) 1E-3;
        if (yrange <= 0)
            yrange = (double) 1E-3;

        if (mode == IS_X)
        {
            grid_step = grid_step_x;
            curr_max = xmax + 0.1 * xrange;
            curr_min = xmin - 0.1 * xrange;
            step = (xmax - xmin) / grid_step;
            
            step = evalStep(xmin, xmax,  grid_step);
            
            is_log = xlog;
        }
        else
        {
            grid_step = grid_step_y;
            curr_max = ymax + (double) 0.1 * yrange;
            curr_min = ymin - (double) 0.1 * yrange;
            step = (ymax - ymin) / grid_step;
            is_log = ylog;
        }

        if (step > 1)
        {
            greater = true;
            while (step / 10. > 1.)
            {
                step /= 10;
                count++;
            }
        }
        else
        {
            greater = false;
            while (step < 1 && step > 0)
            {
                step *= 10;
                count++;
            }
        }
        
        step = (double) ( (int) step);
        num_steps = (int) step;

        if (greater)
            for (i = 0; i < count; i++)
                step *= 10;
        else
            for (i = 0; i < count; i++)
                step /= 10;
        
        curr = (long) (curr_min / step) * step;
        if (curr > curr_min)
            curr -= (long) ( (curr - curr_min) / step) * step;

        while (curr >= curr_min)
            curr -= step;
        
        for (i = 0; i < 50 && curr < curr_max + step; i++)
        {
            val[i] = (long) (curr / step + 0.5) * step;
//Fix per la stampa dello 0
            if (val[i] < step / 100 && val[i] > -step / 100)
                val[i] = 0;

            curr += step;
        }
        if (mode == IS_X)
        {
            x_step = (double) step / num_steps;
            num_x_steps = num_steps;
        }
        else
        {
            y_step = (double) step / num_steps;
            num_y_steps = num_steps;
        }

        return i;
    }

    
    
    
    
    
    
    private int BuildGrid(double val[], int mode, 
                          double xmax, double ymax,
                          double xmin, double ymin, 
                          boolean xlog, boolean ylog)
    {


        if (ymax < ymin)
            ymax = ymin + 1E-10;
        if (xmax < xmin)
            xmax = xmin + 1E-10;
        
        double step, curr, curr_max, curr_min,
               xrange = xmax - xmin,
               yrange = ymax - ymin;
        
        boolean greater = false;
        boolean is_log;
        int grid_step;
        int count = 0, i, num_steps, rem_step = 1;
        Float f;
        
        if (xrange <= 0)
            xrange = (double) 1E-3;
        if (yrange <= 0)
            yrange = (double) 1E-3;

        if (mode == IS_X)
        {
            grid_step = grid_step_x;
            curr_max = xmax + 0.1 * xrange;
            curr_min = xmin - 0.1 * xrange;
            step = (xmax - xmin) / grid_step;
            is_log = xlog;
        }
        else
        {
            grid_step = grid_step_y;
            curr_max = ymax + (double) 0.1 * yrange;
            curr_min = ymin - (double) 0.1 * yrange;

            step = (ymax - ymin) / grid_step;
//	    if(step < 10e-10)
//		step = 10e-10;
            is_log = ylog;

        }

        if (step > 1)
        {
            greater = true;
            while (step / 10. > 1.)
            {
                step /= 10;
                count++;
            }
        }
        else
        {
            greater = false;
            while (step < 1 && step > 0)
            {
                step *= 10;
                count++;
            }
        }
        step = (double) ( (int) step);
        num_steps = (int) step;

        if (greater)
            for (i = 0; i < count; i++)
                step *= 10;
        else
            for (i = 0; i < count; i++)
                step /= 10;
        curr = (long) (curr_min / step) * step;
        if (curr > curr_min)
            curr -= (long) ( (curr - curr_min) / step) * step;

        while (curr >= curr_min)
            curr -= step;
        for (i = 0; i < 50 && curr < curr_max + step; i++)
        {
            val[i] = (long) (curr / step + 0.5) * step;
//Fix per la stampa dello 0
            if (val[i] < step / 100 && val[i] > -step / 100)
                val[i] = 0;

            curr += step;
        }
        if (mode == IS_X)
        {
            x_step = (double) step / num_steps;
            num_x_steps = num_steps;
        }
        else
        {
            y_step = (double) step / num_steps;
            num_y_steps = num_steps;
        }

        return i;
    }

    public void setXaxisHMS(boolean xAxisHMS)
    {
        this.xAxisHMS = xAxisHMS;
    }

    public void GetLimits(Graphics g, Rectangle lim_rect, boolean ylog)
    {
        int label_width, label_height, curr_dim;
        FontMetrics fm;
        Font curr_font;

        fm = g.getFontMetrics();
        if (int_xlabels)
            label_height = 1;
        else
        {
            if (x_label != null)
                label_height = 2 * fm.getHeight();
            else
                label_height = fm.getHeight();
        }
        label_width = 0;
        if (!int_ylabels)
        {
            for (int i = 0; i < y_dim; i++)
            {
                curr_dim = fm.stringWidth(Waveform.ConvertToString(y_values[i],
                    ylog));
                if (label_width < curr_dim)
                    label_width = curr_dim;
            }
            if (y_label != null)
                label_width += fm.getHeight();

        }
        lim_rect.width = label_width;
        lim_rect.height = label_height;

    }

    public void updateValues(String x_label, String y_label, String title,
                             String error, int grid_step_x, int grid_step_y,
                             boolean int_xlabels, boolean int_ylabels,
                             boolean reversed)
    {
        this.reversed = reversed;
        this.x_label = x_label;
        this.y_label = y_label;
        this.title = title;
        this.error = error;
        this.grid_step_x = grid_step_x;
        this.grid_step_y = grid_step_y;
        this.int_xlabels = int_xlabels;
        this.int_ylabels = int_ylabels;
    }

    public void paint(Graphics g, Dimension d, Waveform w, WaveformMetrics wm)
    {
        long dayMilliSeconds = 24 * 60 * 60 * 1000;
        int i, j, dim, num_steps, curr_dim;
        Color prev_col;
        FontMetrics fm;
        double curr_step;
        String curr_string;
        boolean displayDate = true;
        String curr_date_string = null;
        String prev_date_string = "";

        if (reversed)
            g.setColor(Color.white);
        else
            g.setColor(Color.black);

        this.wm = wm;
        fm = g.getFontMetrics();

        if (int_xlabels)
            label_height = 0;
        else
            label_height = /*2 * */ fm.getHeight();

        label_descent = fm.getDescent();
        label_width = 0;

        if (!int_ylabels && wm != null)
        {
            for (i = 0; i < y_dim; i++)
            {
                curr_dim = fm.stringWidth(Waveform.ConvertToString(y_values[i],
                    wm.YLog()));
                if (label_width < curr_dim)
                    label_width = curr_dim;
            }
            if (y_label != null)
                label_width += fm.getHeight();
//	        label_width -= fm.charWidth(' ');
        }

        prev_col = g.getColor();

        if (wm != null)
        {
            for (i = 0; i < y_dim; i++)
            {
                dim = wm.YPixel(y_values[i], d);
                switch (mode)
                {
                    case IS_DOTTED:
                        if (dim <= d.height - label_height)
                            for (j = label_width; j < d.width; j += 4)
                                g.fillRect(j, dim, 1, 1);
                        break;
                    case IS_GRAY:
                        g.setColor(Color.lightGray);
                        if (dim <= d.height - label_height)
                            g.drawLine(label_width, dim, d.width, dim);
                        break;
                    case IS_NONE:
                        if (dim <= d.height - label_height)
                        {
                            g.drawLine(label_width + 3, dim,
                                       d.width / 80 + label_width + 3, dim);
                            g.drawLine(d.width - d.width / 80, dim, d.width,
                                       dim);
                        }
                        if (i == y_dim - 1)
                            break;
                        if (wm.YLog())
                            curr_step = (y_values[i + 1] - y_values[i]) /
                                num_y_steps;
                        else
                            curr_step = y_step;
                        for (j = 1; j <= num_y_steps; j++)
                        {
                            curr_dim = wm.YPixel(y_values[i] + j * curr_step, d);
                            if (curr_dim <= d.height - label_height)
                            {
                                g.drawLine(label_width + 3, curr_dim,
                                           label_width + d.width / 100 + 3,
                                           curr_dim);
                                g.drawLine(d.width - d.width / 100, curr_dim,
                                           d.width, curr_dim);
                            }
                        }
                }

                g.setColor(prev_col);
                if (dim <= d.height - label_height)
                {
                    curr_dim = dim + fm.getHeight() / 2;
                    if ( (curr_dim - fm.getAscent() >= 0) &&
                        (curr_dim + fm.getDescent() <= d.height))
                    {
                        int ylabel_offset = 1;
                        if (y_label != null)
                            ylabel_offset = fm.getHeight();
                        if (int_ylabels)
                        {
                            if (mode == Grid.IS_NONE)
                                ylabel_offset += d.width / 40;
                            else
                                ylabel_offset = 2;
                        }

                        g.drawString(Waveform.ConvertToString(y_values[i],
                            wm.YLog()), ylabel_offset + 1, curr_dim);
                    }
                }
            }
            int prevIdx;
            String currStringSubSec = "";
            for (i = prevIdx = 0; i < x_dim; i++)
            {
                dim = wm.XPixel(x_values[i], d);
                switch (mode)
                {
                    case IS_DOTTED:
                        if (dim >= label_width)
                            for (j = 0; j < d.height - label_height; j += 4)
                                g.fillRect(dim, j, 1, 1);
                        break;
                    case IS_GRAY:
                        g.setColor(Color.lightGray);
                        if (dim >= label_width)
                            g.drawLine(dim, 0, dim, d.height - label_height);
                        break;
                    case IS_NONE:
                        if (dim >= label_width)
                        {
                            g.drawLine(dim, 2, dim, d.height / 40);
                            g.drawLine(dim,
                                       d.height - label_height - d.height / 40,
                                       dim,
                                       d.height - label_height);
                        }
                        if (i == x_dim - 1)
                            break;
                        if (wm.XLog())
                            curr_step = (x_values[i + 1] - x_values[i]) /
                                num_x_steps;
                        else
                            curr_step = x_step;
                        for (j = 1; j <= num_x_steps; j++)
                        {
                            double val = x_values[i] + j * curr_step;
                            curr_dim = wm.XPixel( val, d);
                            if (curr_dim >= label_width)
                            {
                                g.drawLine(curr_dim, 2, curr_dim, d.height / 80);
                                g.drawLine(curr_dim,
                                           d.height - label_height -
                                           d.height / 80,
                                           curr_dim, d.height - label_height);
                            }
                        }
                        g.drawRect(label_width + 3, 2,
                                   d.width - label_width - 3,
                                   d.height - label_height - 2);
                }

                g.setColor(prev_col);
                if(xAxisHMS)
                {
                    try
                    {
                        long datel = (long) x_values[i];
                        DateFormat df = new SimpleDateFormat("HH:mm:ss");
                        //GABdf.setTimeZone(new SimpleTimeZone(0, "GMT"));
                        //df.setTimeZone(TimeZone.getDefault());

                        DateFormat dfSubSec = new SimpleDateFormat("HH:mm:ss.SSS");
                        //--dfSubSec.setTimeZone(new SimpleTimeZone(0, "GMT"));
                        //GABdfSubSec.setTimeZone(TimeZone.getDefault());

                        if( datel <= 86400000 )
                        {
                           // if the date to convert is in the date 1 Jan 1970
                           // is whown only the huor and the time xone must be set
                           // to GTM to avoid to add the hours of the time zone
                            df.setTimeZone(new SimpleTimeZone(0, "GMT"));
                            dfSubSec.setTimeZone(new SimpleTimeZone(0, "GMT"));

                        }

                        Date date = new Date();
                        date.setTime(datel);
                        currStringSubSec = dfSubSec.format(date).toString();
                        curr_string = df.format(date).toString();

                        DateFormat df1 = new SimpleDateFormat("d-MMM-yyyy");

                        //--df1.setTimeZone(new SimpleTimeZone(0, "GMT"));
                        //GABdf1.setTimeZone(TimeZone.getDefault());

                        String new_date_string = df1.format(date).toString();

                        if(i == 0 || !new_date_string.equals(prev_date_string))
                        {
                            curr_date_string = prev_date_string = new_date_string;
                        }
                        else
                            curr_date_string = null;


                        if( i < x_dim - 1 )
                        {
                            long num_day  = calculateDifference(new Date((long)x_values[i]), new Date((long)x_values[i+1]));
                            if(num_day != 0)
                            {

                                Calendar ca = Calendar.getInstance();
                                //ca.setTimeZone(TimeZone.getTimeZone("GMT+00"));

                                ca.setTimeInMillis((long)x_values[i]);
                                ca.set(ca.get(Calendar.YEAR), ca.get(Calendar.MONTH), ca.get(Calendar.DAY_OF_MONTH)+1, 0, 0);
                                for(int dd = 0; dd < num_day; dd++)
                                {
                                   long timeMillis = ca.getTimeInMillis();
                                    if (timeMillis < xmax)
                                    {
                                        Color c = g.getColor();
                                        g.setColor(Color.BLUE);
                                        curr_dim = wm.XPixel( (double)
                                            timeMillis, d);
                                        if (curr_dim >= label_width)
                                        {
                                            //  g.drawLine(curr_dim, 0, curr_dim,d.height - label_height);                                                                    case IS_DOTTED:
                                            for (j = 0;
                                                 j < d.height - label_height;
                                                 j += 7)
                                                g.fillRect(curr_dim, j, 1, 5);
                                        }
                                        g.setColor(c);
                                    }
                                    ca.set(ca.get(Calendar.YEAR), ca.get(Calendar.MONTH), ca.get(Calendar.DAY_OF_MONTH)+1, 0, 0);

                                }
                            }
                         }
                    }
                    catch (Exception exc)
                    {
                        curr_string = Waveform.ConvertToString(x_values[i],
                            wm.XLog());
                    }
                } else
                    curr_string = Waveform.ConvertToString(x_values[i], wm.XLog());


                curr_dim = dim - fm.stringWidth(curr_string) / 2;
                if (curr_dim >= label_width &&
                    dim + fm.stringWidth(curr_string) / 2 < d.width)
                {
                    if(xAxisHMS && i > 0 && x_values[i] - x_values[prevIdx] < 1000)
                        g.drawString(currStringSubSec, curr_dim,
                                 d.height - fm.getHeight() / 10 - label_descent);
                    else
                        g.drawString(curr_string, curr_dim,
                                 d.height - fm.getHeight() / 10 - label_descent);
                    prevIdx = i;
                }
                if(curr_date_string != null)
                {
                    curr_dim = dim - fm.stringWidth(curr_date_string) / 2;
                    if (curr_dim >= label_width &&
                        dim + fm.stringWidth(curr_string) / 2 < d.width)
                    {
                        g.drawString(curr_date_string, curr_dim,
                                     d.height - fm.getHeight() - 2*fm.getHeight() / 10 - label_descent);
                    }

                }
            }

        } //End if check is_image

        if (x_label != null && x_label.length() != 0)
            g.drawString(x_label, (d.width - fm.stringWidth(x_label)) / 2,
                         d.height - label_descent - fm.getHeight());
        if (y_label != null && y_label.length() != 0)
        {
            Graphics2D g2d = (Graphics2D) g;
            double x_tra = 4 + fm.getHeight();
            double y_tra = (d.height + fm.stringWidth(y_label)) / 2;
            double angle = Math.PI / 2.0;

            g2d.translate(x_tra, y_tra);
            g2d.rotate( -angle);
            g2d.drawString(y_label, 0, 0);
            g2d.rotate(angle);
            g2d.translate( -x_tra, -y_tra);
        }
        if (title != null && title.length() != 0)
            g.drawString(title, (d.width - fm.stringWidth(title)) / 2,
                         fm.getAscent() + d.height / 40);
        if (error != null && error.length() != 0)
        {
            int y_pos = 0;
            if (title != null && title.trim().length() != 0)
                y_pos = fm.getHeight();
            g.drawString(error, (d.width - fm.stringWidth(error)) / 2,
                         y_pos + fm.getAscent() + d.height / 40);
        }
    }

    public Rectangle GetBoundingBox(Dimension d)
    {
        return new Rectangle(label_width, 0, d.width - label_width + 1,
                             d.height - label_height + 1);
    }

    public void setLabels(String title, String x_label, String y_label)
    {
        this.title = title;
        this.x_label = x_label;
        this.y_label = y_label;
    }

    public long calculateDifference(Date a, Date b)
    {
        Calendar cal1 = Calendar.getInstance();
        //GABcal1.setTimeZone(TimeZone.getTimeZone("GMT+00"));
        cal1.setTime(a);
        cal1.set(cal1.get(Calendar.YEAR), cal1.get(Calendar.MONTH), cal1.get(Calendar.DAY_OF_MONTH), 23, 59, 59);
        Calendar cal2 = Calendar.getInstance();
        //GABcal2.setTimeZone(TimeZone.getTimeZone("GMT+00"));
        cal2.setTime(b);
        cal2.set(cal2.get(Calendar.YEAR), cal2.get(Calendar.MONTH), cal2.get(Calendar.DAY_OF_MONTH), 0, 0, 0);
        long diffMillis = cal2.getTimeInMillis() - cal1.getTimeInMillis();
        if(diffMillis < 0) return 0;
        return 1 + diffMillis/(1000*3600*24);
    }

    public static void main(String args[])
    {
        double step = Grid.evalStep(0., 1., 10);
        step = Grid.evalStep(0., 10., 10);
        step = Grid.evalStep(0., 100., 5);
    }
}
