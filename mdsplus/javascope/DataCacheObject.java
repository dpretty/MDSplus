/* $Id: DataCacheObject.java,v 1.2 2002/04/26 13:14:29 jgk Exp $ */
import java.io.Serializable;

class DataCacheObject implements Serializable
{
    float data[];
    float x[];
    float y[];
    float up_err[];
    float low_err[];
    int dimension;
    String title;
    String x_label;
    String y_label;
}
