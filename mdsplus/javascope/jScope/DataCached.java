package jScope;

/* $Id: DataCached.java,v 1.1 2010/09/03 09:58:16 manduchi Exp $ */
import java.io.Serializable;

public class DataCached implements Serializable
 {
    String expression;
    String experiment;
    long    shot;
    Object data;
    
    DataCached(String expression, String experiment, long shot, Object data)
    {
        this.expression = expression;
        this.shot = shot;
        this.experiment = experiment;
        this.data = data;
    }
    
    public boolean equals(String expression, String experiment, long shot)
    {
        return ( this.expression.equals(expression) && 
                 this.shot == shot && 
                 this.experiment.equals(experiment));
    }
 }
