/* $Id: DataCached.java,v 1.2 2002/04/26 13:14:29 jgk Exp $ */
import java.io.Serializable;

public class DataCached implements Serializable
 {
    String expression;
    String experiment;
    int    shot;
    Object data;
    
    DataCached(String expression, String experiment, int shot, Object data)
    {
        this.expression = expression;
        this.shot = shot;
        this.experiment = experiment;
        this.data = data;
    }
    
    public boolean equals(String expression, String experiment, int shot)
    {
        return ( this.expression.equals(expression) && 
                 this.shot == shot && 
                 this.experiment.equals(experiment));
    }
 }
