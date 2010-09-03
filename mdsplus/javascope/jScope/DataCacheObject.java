package jScope;

/* $Id: DataCacheObject.java,v 1.1 2010/09/03 09:58:16 manduchi Exp $ */
import java.io.Serializable;

class DataCacheObject implements Serializable
{
    float data[];
    float x[];
    double x_double[];
    float y[];
    float up_err[];
    float low_err[];
    int dimension;
    String title;
    String x_label;
    String y_label;
    String z_label;
}
