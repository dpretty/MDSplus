/* $Id: jScopeDefaultValues.java,v 1.5 2002/07/26 10:56:03 manduchi Exp $ */
class jScopeDefaultValues 
{
   long	  shots[];
   String xmin, xmax, ymax, ymin;
   String title_str, xlabel, ylabel;
   String experiment_str, shot_str;
   String upd_event_str, def_node_str;
   boolean is_evaluated = false;
   String public_variables = null;

   public void Reset()
   {
        shots = null;
        xmin = xmax = ymax = ymin = null;
        title_str = xlabel = ylabel = null;
        experiment_str = shot_str = null;
        upd_event_str = def_node_str = null;
        is_evaluated = false;
   }
}
