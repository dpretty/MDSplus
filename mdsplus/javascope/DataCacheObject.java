/* $Id: DataCacheObject.java,v 1.3 2002/07/26 10:55:55 manduchi Exp $ */
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
