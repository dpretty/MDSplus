import javax.swing.*;
import java.beans.*;
import java.awt.*;
import java.lang.reflect.*;
import java.awt.event.*;
import java.util.*;

public class DeviceChoiceCustomizer extends DeviceCustomizer implements Customizer
{
    DeviceChoice bean = null;
    PropertyChangeSupport listeners = new PropertyChangeSupport(this);
    TextField labelString, identifier;
    TextArea itemsArea, codesArea;
    Label itemsLabel, codesLabel;
    Choice nids, mode;
    Button doneButton;
    Checkbox showState;
    
    public DeviceChoiceCustomizer()
    {
    }        
    public void setObject(Object o)
    {
        bean = (DeviceChoice)o;

        setLayout(new BorderLayout());
        Panel jp = new Panel();
        jp.setLayout(new BorderLayout());
        Panel jp1 = new Panel();
        jp1.add(new Label("Label: "));
        jp1.add(labelString = new TextField(30));
        labelString.setText(bean.getLabelString());
        jp1.add(showState = new Checkbox("Show state: ", bean.getShowState()));
        jp1.add(new Label("Offset nid: "));
        jp1.add(nids = new Choice());
        
        String names[] = getDeviceFields();
        
        if(names != null)
        for(int i = 0; i < names.length; i++)
            nids.add(names[i]);
        int idx = bean.getOffsetNid();
        if(idx > 0 && idx < names.length)
        nids.select(idx - 1);
        jp.add(jp1, "North");
        jp1 = new Panel();
        jp1.add(new Label("Mode: "));
        jp1.add(mode = new Choice());
        mode.add("String");
        mode.add("Integer");
        mode.add("Float");
        mode.add("Code");
        mode.addItemListener(new ItemListener()
        {
            public void itemStateChanged(ItemEvent e)
            {
                int curr_idx = mode.getSelectedIndex();
                if(curr_idx == 3)
                {
                    codesLabel.setEnabled(true);
                    codesArea.setEnabled(true);
                }
                else
                {
                    codesLabel.setEnabled(false);
                    codesArea.setEnabled(false);
                }
        }});
        boolean convert = bean.getConvert();
        String [] items = bean.getChoiceItems();
        int [] choiceIntValues = bean.getChoiceIntValues();
        float [] choiceFloatValues = bean.getChoiceFloatValues();
        if(convert)
            mode.select(3);
        else if(choiceIntValues != null)
            mode.select(1);
        else if(choiceFloatValues != null)
            mode.select(2);
        else mode.select(0);
        jp1.add(itemsLabel = new Label("Items: "));
        jp1.add(itemsArea = new TextArea(5, 15));
        if(items != null)
            for(int i = 0; i < items.length; i++)
                itemsArea.append(items[i] + "\n");
        jp1.add(codesLabel = new Label("Codes: "));
        jp1.add(codesArea = new TextArea(5, 4));
        if(convert && choiceIntValues != null)
            for(int i = 0; i < choiceIntValues.length; i++)
                codesArea.append((new Integer(choiceIntValues[i])).toString() + "\n");
        codesLabel.setEnabled(false);
        codesArea.setEnabled(false);
                
        jp.add(jp1, "Center");
        jp1 = new Panel();
        jp1.add(new Label("Opt. identifier: "));
        jp1.add(identifier = new TextField(bean.getIdentifier(), 20));
        jp.add(jp1, "South");
        
        add(jp, "Center");
        jp = new Panel();
        jp.add(doneButton = new Button("Apply"));
        doneButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                bean.setLabelString(labelString.getText());
                bean.setShowState(showState.getState());
                bean.setOffsetNid(nids.getSelectedIndex() + 1);
                int curr_idx = mode.getSelectedIndex();
                switch(curr_idx) 
                {
                    case 0: //String
                        bean.setConvert(false);
                        bean.setChoiceItems(convertText(itemsArea.getText()));
                        bean.setChoiceIntValues(null);
                        bean.setChoiceFloatValues(null);
                        break;
                    case 1: //Integer
                        bean.setConvert(false);
                        bean.setChoiceItems(convertText(itemsArea.getText()));
                        bean.setChoiceIntValues(convertInt(itemsArea.getText()));
                        bean.setChoiceFloatValues(null);
                        break;
                    case 2: //Float
                        bean.setConvert(false);
                        bean.setChoiceItems(convertText(itemsArea.getText()));
                        bean.setChoiceIntValues(null);
                        bean.setChoiceFloatValues(convertFloat(itemsArea.getText()));
                        break;
                    case 3: //Code    
                        bean.setConvert(true);
                        bean.setChoiceItems(convertText(itemsArea.getText()));
                        bean.setChoiceIntValues(null);
                        bean.setChoiceFloatValues(null);
                        bean.setChoiceIntValues(convertInt(codesArea.getText()));
                        break;
                }
                
                bean.setIdentifier(identifier.getText());
                listeners.firePropertyChange(null, null, null);
            }
        });
        add(jp,"South");
    }
    
    protected String [] convertText(String inText)
    {
        int i = 0;
        StringTokenizer st = new StringTokenizer(inText, "\n\r");
        String []items = new String[st.countTokens()];
        while(st.hasMoreTokens())
            items[i++] = st.nextToken();
        return items;
    }
    
    protected int [] convertInt(String inText)
    {
        String [] items = convertText(inText);
        int out[] = new int[items.length];
        for(int i = 0; i < items.length; i++)
            out[i] = (new Integer(items[i])).intValue();
        return out;
    }
    
    protected float [] convertFloat(String inText)
    {
        String [] items = convertText(inText);
        float out[] = new float[items.length];
        for(int i = 0; i < items.length; i++)
            out[i] = (new Float(items[i])).floatValue();
        return out;
    }
    
    
    
    public void addPropertyChangeListener(PropertyChangeListener l)
    {
        listeners.addPropertyChangeListener(l);
    }
    
    public void removePropertyChangeListener(PropertyChangeListener l)
    {
        listeners.removePropertyChangeListener(l);
    }
  }
        
